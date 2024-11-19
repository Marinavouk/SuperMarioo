#include "Pch.h"
#include "Player.h"

#include "Application.h"
#include "Utilities/CollisionUtilities.h"
bool CPlayer::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{

	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	const SDL_FPoint frameSize = { 64.0f, 128.0f };

	m_pAnimatorIdle = new CAnimator;
	m_pAnimatorWalking = new CAnimator;
	m_pAnimatorRunning = new CAnimator;
	m_pAnimatorJumping = new CAnimator;
	m_pAnimatorDying = new CAnimator;
	m_pAnimatorIdle->Set(m_pTexture, 21, 0, 20, 1, frameSize, 7.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorWalking->Set(m_pTexture, 21, 2, 4, 1, frameSize, 8.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorRunning->Set(m_pTexture, 21, 2, 4, 1, frameSize, 14.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorJumping->Set(m_pTexture, 21, 6, 6, 1, frameSize, 8.0f, false, CAnimator::EDirection::FORWARD);
	m_pAnimatorDying->Set(m_pTexture, 21, 15, 15, 1, frameSize, 8.0f, false, CAnimator::EDirection::FORWARD);

	ActivateAnimator(m_pAnimatorIdle);

	m_pTexture->SetSize({ frameSize.x * m_Scale, frameSize.y * m_Scale });
	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());

	m_Rectangle = { position.x, position.y, frameSize.x * m_Scale, frameSize.y * m_Scale };

	m_HorizontalCollider = { m_Rectangle.x + m_HorizontalColliderOffset.x,	m_Rectangle.y + m_HorizontalColliderOffset.y,	18.0f * m_Scale, 40.0f * m_Scale };
	m_VerticalCollider = { m_Rectangle.x + m_VerticalColliderOffset.x,		m_Rectangle.y + m_VerticalColliderOffset.y,		10.0f * m_Scale, 64.0f * m_Scale };

	m_Collider = { m_VerticalCollider.x, m_VerticalCollider.y, m_VerticalCollider.w, m_VerticalCollider.h };

    return true;
}

void CPlayer::Destroy(void)
{
	m_pDyingCallback   = nullptr;

	delete m_pAnimatorDying;
	delete m_pAnimatorJumping;
	delete m_pAnimatorRunning;
	delete m_pAnimatorWalking;
	delete m_pAnimatorIdle;
	m_pAnimatorIdle    = nullptr;
	m_pAnimatorWalking = nullptr;
	m_pAnimatorRunning = nullptr;
	m_pAnimatorJumping = nullptr;
	m_pAnimatorDying   = nullptr;

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

void CPlayer::Render(void)
{
	CGameObject::Render();
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
	if (m_HorizontalCollider.x < 0.0f)
	{
		m_Rectangle.x = -m_HorizontalColliderOffset.x;

		m_Velocity.x = 0.0f;
	}

	// Make sure that the player can't leave the right edge of the window
	else if (m_HorizontalCollider.x > (windowSize.x - m_HorizontalCollider.w))
	{
		const float rightOffset = m_Rectangle.w - (m_HorizontalCollider.w + m_HorizontalColliderOffset.x);

		m_Rectangle.x = windowSize.x - (m_Rectangle.w - rightOffset);

		m_Velocity.x = 0.0f;
	}

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
		
		m_IsJumping = true; // Need to confirm if it's working
	}


	// Held keys

	m_IsRunning = inputHandler.KeyHeld(SDL_SCANCODE_S);

	if (inputHandler.KeyHeld(SDL_SCANCODE_LEFT) && !inputHandler.KeyHeld(SDL_SCANCODE_RIGHT))
	{
		const float acceleration = (m_IsRunning ? m_AccelerationSpeedRunning : m_AccelerationSpeedWalking) * deltaTime;
		const float maxVelocity = (m_IsRunning ? -m_MaxRunningVelocity : -m_MaxWalkingVelocity);

		m_Velocity.x = std::max(m_Velocity.x - acceleration, maxVelocity);

		m_pTexture->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

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

void CPlayer::HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime)
{
}

void CPlayer::HandleEnemyCollision(const GameObjectList& enemies, const float deltaTime)
{
}

bool CPlayer::ResolveObstacleXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	return false;
}

void CPlayer::SyncColliders(void)
{
	m_HorizontalCollider.x = m_Rectangle.x + m_HorizontalColliderOffset.x;
	m_HorizontalCollider.y = m_Rectangle.y + m_HorizontalColliderOffset.y;
	m_VerticalCollider.x = m_Rectangle.x + m_VerticalColliderOffset.x;
	m_VerticalCollider.y = m_Rectangle.y + m_VerticalColliderOffset.y;

	m_Collider = { m_VerticalCollider.x, m_VerticalCollider.y, m_VerticalCollider.w, m_VerticalCollider.h };
}

void CPlayer::ActivateAnimator(CAnimator* animator)
{
	if (m_pCurrentAnimator != animator)
	{
		m_pCurrentAnimator = animator;
		m_pCurrentAnimator->Reset();
	}
}
 
