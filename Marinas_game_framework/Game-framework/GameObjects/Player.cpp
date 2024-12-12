#include "Pch.h"
#include "Player.h"

#include "Application.h"
#include "Utilities/CollisionUtilities.h"

bool CPlayer::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	SDL_FPoint frameSize = { 28.0f, 28.0f };

	m_pAnimatorIdle = new CAnimator;
	m_pAnimatorWalking = new CAnimator;
	m_pAnimatorRunning = new CAnimator;
	m_pAnimatorJumping = new CAnimator;
	m_pAnimatorDying = new CAnimator;
	m_pAnimatorIdle->Set(m_pTexture, 1, 0, 0, 0, frameSize, 8.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorWalking->Set(m_pTexture, 3, 1, 3, 0, frameSize, 8.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorRunning->Set(m_pTexture, 3, 1, 3, 0, frameSize, 16.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorJumping->Set(m_pTexture, 1, 5, 5, 0, frameSize, 8.0f, false, CAnimator::EDirection::FORWARD);
	m_pAnimatorDying->Set(m_pTexture, 1, 7, 7, 0, frameSize, 1.0f, false, CAnimator::EDirection::FORWARD);

	ActivateAnimator(m_pAnimatorIdle);

	m_pTexture->SetSize({ frameSize.x * m_Scale, frameSize.y * m_Scale });
	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());

	m_Rectangle = { position.x, position.y, frameSize.x * m_Scale, frameSize.y * m_Scale };

	m_HorizontalCollider = { m_Rectangle.x + m_HorizontalColliderOffset.x,	m_Rectangle.y + m_HorizontalColliderOffset.y,	15.0f * m_Scale, 9.0f * m_Scale };
	m_VerticalCollider = { m_Rectangle.x + m_VerticalColliderOffset.x,		m_Rectangle.y + m_VerticalColliderOffset.y,		10.0f * m_Scale, 24.0f * m_Scale };

	m_Collider = { m_VerticalCollider.x, m_VerticalCollider.y, m_VerticalCollider.w, m_VerticalCollider.h };

	return true;
}

void CPlayer::Destroy(void)
{
	m_pDyingCallback = nullptr;

#define DELETE_ANIMATOR(Animator) delete Animator; Animator = nullptr;

	DELETE_ANIMATOR(m_pAnimatorDying);
	DELETE_ANIMATOR(m_pAnimatorJumping);
	DELETE_ANIMATOR(m_pAnimatorRunning);
	DELETE_ANIMATOR(m_pAnimatorWalking);
	DELETE_ANIMATOR(m_pAnimatorIdle);

#undef DELETE_ANIMATOR

	m_pCurrentAnimator = nullptr;

	CGameObject::Destroy();
}

void CPlayer::Kill(void)
{
	CGameObject::Kill();

	ActivateAnimator(m_pAnimatorDying);

	m_Velocity = { 0.0f, 0.0f };

	m_State = EState::DEAD;

	if (m_pDyingCallback)
		m_pDyingCallback();
}

void CPlayer::RenderDebug(void)
{
	CGameObject::RenderDebug();

	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 255, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_HorizontalCollider);

	SDL_SetRenderDrawColor(renderer, 0, 200, 200, 255);
	SDL_RenderDrawRectF(renderer, &m_VerticalCollider);
}

void CPlayer::Update(const float deltaTime)
{
	m_Velocity.y = std::min(m_Velocity.y + m_Gravity * deltaTime, m_MaxFallVelocity);

	m_Rectangle.x += m_Velocity.x * deltaTime;
	m_Rectangle.y += m_Velocity.y * deltaTime;

	SyncColliders();

	const SDL_FPoint windowSize = m_pApplication->GetWindowSize();

	// Make sure that the player can't leave the left edge of the window
	/*if (m_HorizontalCollider.x < 0.0f)
	{
		m_Rectangle.x = -m_HorizontalColliderOffset.x;

		m_Velocity.x = 0.0f;
	}*/

	// Make sure that the player can't leave the right edge of the window
	/*else if (m_HorizontalCollider.x > (windowSize.x - m_HorizontalCollider.w))
	{
		const float rightOffset = m_Rectangle.w - (m_HorizontalCollider.w + m_HorizontalColliderOffset.x);

		m_Rectangle.x = windowSize.x - (m_Rectangle.w - rightOffset);

		m_Velocity.x = 0.0f;
	}*/

	// Left side of the window
	if ((m_Rectangle.x + (m_Rectangle.w * 0.5f)) < 0.0f)
		m_Rectangle.x = windowSize.x - (m_Rectangle.w * 0.5f);

	// Right side of the window
	else if ((m_Rectangle.x + (m_Rectangle.w * 0.5f)) > windowSize.x)
		m_Rectangle.x = -(m_Rectangle.w * 0.5f);


	if (m_State == EState::ALIVE)
	{
		const SDL_FPoint windowSize = m_pApplication->GetWindowSize();

		if (m_Rectangle.y > windowSize.y - m_Rectangle.h)
		{
			m_Rectangle.y = windowSize.y - m_Rectangle.h;

			m_Velocity.y = 0.0f;

			if (m_IsJumping)
			{
				if ((m_pCurrentAnimator != m_pAnimatorIdle) && (m_HorizontalDirection == EMovementState::IDLE) && (m_VerticalDirection == EMovementState::IDLE))
					ActivateAnimator(m_pAnimatorIdle);

				else
				{
					if (m_HorizontalDirection != EMovementState::IDLE)
					{
						if (m_IsRunning)
							ActivateAnimator(m_pAnimatorRunning);

						else
							ActivateAnimator(m_pAnimatorWalking);
					}
				}
			}

			m_IsJumping = false;
		}
	}

	else if (m_State == EState::DEAD)
	{
		const SDL_FPoint windowSize = m_pApplication->GetWindowSize();

		if (m_Rectangle.y > windowSize.y - m_Rectangle.h)
		{
			m_Rectangle.y = windowSize.y - m_Rectangle.h;

			//Add 1 inch velocity upwards when mario dies

			m_Velocity.y = 0.0f;
		}
	}

	SyncColliders();

	if (m_pCurrentAnimator)
		m_pCurrentAnimator->Update(deltaTime);
}

void CPlayer::HandleInput(const float deltaTime)
{
	if (m_IsDead)
		return;

	CInputHandler& inputHandler = m_pApplication->GetInputHandler();

	// Pressed keys

	if (inputHandler.KeyPressed(SDL_SCANCODE_SPACE) && !m_IsJumping)
	{
		m_Velocity.y = -m_JumpStrength;

		m_pCurrentAnimator = m_pAnimatorJumping;
		m_pCurrentAnimator->Reset();

		m_IsJumping = true;
	}

	// Held keys

	m_IsRunning = inputHandler.KeyHeld(SDL_SCANCODE_S);

	if (inputHandler.KeyHeld(SDL_SCANCODE_LEFT) && !inputHandler.KeyHeld(SDL_SCANCODE_RIGHT))
	{
		const float acceleration = (m_IsRunning ? m_AccelerationSpeedRunning : m_AccelerationSpeedWalking) * deltaTime;
		const float maxVelocity = (m_IsRunning ? -m_MaxRunningVelocity : -m_MaxWalkingVelocity);

		m_Velocity.x = std::max(m_Velocity.x - acceleration, maxVelocity);

		m_pTexture->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

		m_FlipMethod = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;

		m_HorizontalDirection = EMovementState::MOVING_LEFT;

		if (!m_IsJumping)
		{
			if (m_IsRunning)
				ActivateAnimator(m_pAnimatorRunning);

			else
				ActivateAnimator(m_pAnimatorWalking);
		}
	}

	else if (inputHandler.KeyHeld(SDL_SCANCODE_RIGHT) && !inputHandler.KeyHeld(SDL_SCANCODE_LEFT))
	{
		const float acceleration = (m_IsRunning ? m_AccelerationSpeedRunning : m_AccelerationSpeedWalking) * deltaTime;
		const float maxVelocity = (m_IsRunning ? m_MaxRunningVelocity : m_MaxWalkingVelocity);

		m_Velocity.x = std::min(m_Velocity.x + acceleration, maxVelocity);

		m_pTexture->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_NONE);

		m_FlipMethod = SDL_RendererFlip::SDL_FLIP_NONE;

		m_HorizontalDirection = EMovementState::MOVING_RIGHT;

		if (!m_IsJumping)
		{
			if (m_IsRunning)
				ActivateAnimator(m_pAnimatorRunning);

			else
				ActivateAnimator(m_pAnimatorWalking);
		}
	}

	else
	{
		const float speed = (m_IsRunning ? m_DeaccelerationSpeedRunning : m_DeaccelerationSpeedWalking) * deltaTime;

		if (m_Velocity.x < 0.0f) m_Velocity.x = std::min(0.0f, m_Velocity.x + speed);
		else if (m_Velocity.x > 0.0f) m_Velocity.x = std::max(0.0f, m_Velocity.x - speed);
	}

	// Released keys

	if (inputHandler.KeyReleased(SDL_SCANCODE_LEFT) && (m_HorizontalDirection == EMovementState::MOVING_LEFT))
	{
		m_HorizontalDirection = EMovementState::IDLE;

		if ((m_VerticalDirection == EMovementState::IDLE) && !m_IsJumping)
			ActivateAnimator(m_pAnimatorIdle);
	}

	else if (inputHandler.KeyReleased(SDL_SCANCODE_RIGHT) && (m_HorizontalDirection == EMovementState::MOVING_RIGHT))
	{
		m_HorizontalDirection = EMovementState::IDLE;

		if ((m_VerticalDirection == EMovementState::IDLE) && !m_IsJumping)
			ActivateAnimator(m_pAnimatorIdle);
	}

	if ((m_HorizontalDirection == EMovementState::IDLE) && (m_VerticalDirection == EMovementState::IDLE) && !m_IsJumping)
		ActivateAnimator(m_pAnimatorIdle);
}

void CPlayer::HandleTilemapCollision(const CTilemap::TileColliders& tilemapColliders, const float deltaTime)
{
	const SDL_FPoint moveAmount = {m_Velocity.x * deltaTime, m_Velocity.y * deltaTime};

	for (const SDL_FRect& collider : tilemapColliders)
	{
		if (ResolveObstacleXCollision(collider, moveAmount))
			break;

		if (ResolveObstacleYCollision(collider, moveAmount))
			break;
	}
}

void CPlayer::HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime)
{
	static bool isRandSeeded = false;
	if (!isRandSeeded)
	{
		srand(static_cast<unsigned>(time(0)));
		isRandSeeded = true;
	}

	const SDL_FPoint moveAmount = { m_Velocity.x * deltaTime, m_Velocity.y * deltaTime };

	for (size_t i = 0; i < obstacles.size(); ++i)
	{
		CGameObject* pipe = obstacles[i];

		if (ResolveObstacleXCollision(pipe->GetCollider(), moveAmount) || ResolveObstacleYCollision(pipe->GetCollider(), moveAmount))
		{
			
			size_t randomIndex;
			do
			{
				randomIndex = rand() % obstacles.size();
			} while (randomIndex == i);

			CGameObject* randomPipe = obstacles[randomIndex];

			m_Rectangle.x = randomPipe->GetCollider().x + randomOffset; // Offset to place near the pipe
			m_Rectangle.y = randomPipe->GetCollider().y;

			return;
		}
	}
}

void CPlayer::HandleEnemyCollision(const GameObjectList& enemies, const float deltaTime)
{
	if (m_IsDead)
		return;

	const SDL_FPoint	moveAmount = { m_Velocity.x * deltaTime, m_Velocity.y * deltaTime };
	bool				hasCollided = false;

	for (CGameObject* enemy : enemies)
	{
		if (enemy->GetIsDead())
			continue;

		if (ResolveEnemyXCollision(enemy->GetCollider(), moveAmount))
		{
			hasCollided = true;

			break;
		}


		if (hasCollided)
		{
			if (m_CurrentHealth > 0)
			{
				m_CurrentHealth--;

				if (m_CurrentHealth == 0)
				{
					if (m_CurrentHealth == 0)
						Kill();
				}
			}
		}
	}
}

bool CPlayer::ResolveObstacleXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollided = false;

	// The player is moving to the left
	if (moveAmount.x < 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{ 
			m_Rectangle.x += intersection.w;

			m_Velocity.x = 0.0f;

			hasCollided = true;
		}
	}

	// The player is moving to the right
	else if (moveAmount.x > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x -= intersection.w;

			m_Velocity.x = 0.0f;

			hasCollided = true;
		}
	}

	if (hasCollided)
		SyncColliders();

	if (moveAmount.y > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x += ((m_HorizontalCollider.x < collider.x) ? -intersection.w : intersection.w);

			SyncColliders();

			m_Velocity.x = 0.0f;

			hasCollided = true;
		}
	}

	return hasCollided;
}

bool CPlayer::ResolveObstacleYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollided = false;

	// The player is moving up (jumping)
	if (moveAmount.y < 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_VerticalCollider, collider, &intersection))
		{
			m_Rectangle.y += intersection.h;

			m_Velocity.y = 0.0f;

			hasCollided = true;
		}
	}

	// The player is moving down
	else if (moveAmount.y > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_VerticalCollider, collider, &intersection))
		{
			m_Rectangle.y -= intersection.h;

			m_Velocity.y = 0.0f;

			m_IsJumping = false;

			hasCollided = true;
		}
	}

	if (hasCollided)
		SyncColliders();

	return hasCollided;
}

bool CPlayer::ResolveEnemyXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollidedX = false;

	// The player is moving to the left
	if (moveAmount.x < 0.0f)
	{
		SDL_FRect intersection = { 0.0f, 0.0f, 0.0f, 0.0f };

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x += intersection.w;

			m_Velocity.x = 0.0f;

			hasCollidedX = true;
		}
	}

	// The player is moving to the right
	else if (moveAmount.x > 0.0f)
	{
		SDL_FRect intersection = { 0.0f, 0.0f, 0.0f, 0.0f };

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x -= intersection.w;

			m_Velocity.x = 0.0f;

			hasCollidedX = true;
		}
	}

	if (hasCollidedX)
		SyncColliders();

	return hasCollidedX;
}

bool CPlayer::ResolveEnemyYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	return false;
}

void CPlayer::SyncColliders(void)
{
	m_HorizontalCollider.x = m_Rectangle.x + m_HorizontalColliderOffset.x;
	m_HorizontalCollider.y = m_Rectangle.y + m_HorizontalColliderOffset.y;
	m_VerticalCollider.x = m_Rectangle.x + m_VerticalColliderOffset.x;
	m_VerticalCollider.y = m_Rectangle.y + m_VerticalColliderOffset.y;

	m_Collider = { m_VerticalCollider.x, m_VerticalCollider.y , m_VerticalCollider.w, m_VerticalCollider.h };
}

void CPlayer::ActivateAnimator(CAnimator* animator)
{
	if (m_pCurrentAnimator != animator)
	{
		m_pCurrentAnimator = animator;
		m_pCurrentAnimator->Reset();
	}
}