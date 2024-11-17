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




    return false;
}

void CPlayer::Destroy(void)
{
}

void CPlayer::Kill(void)
{
}

void CPlayer::Render(void)
{
}

void CPlayer::RenderDebug(void)
{
}

void CPlayer::Update(const float deltaTime)
{
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

		if (!m_IsAttacking)
		{
			m_pCurrentAnimator = m_pAnimatorJumping;
			m_pCurrentAnimator->Reset();
		}

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

