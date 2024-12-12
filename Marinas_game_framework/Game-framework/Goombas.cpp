#include "Pch.h"
#include "Goombas.h"

#include "Application.h"
#include "Utilities/CollisionUtilities.h"

bool CGoombas::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	const SDL_FPoint frameSize = { 28.0f, 28.0f };

	m_pAnimatorWalking = new CAnimator;
	m_pAnimatorDying = new CAnimator;
	m_pAnimatorWalking->Set(m_pTexture, 2, 0, 1, 0, frameSize, 7.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorDying->Set(m_pTexture, 1, 2, 2, 0, frameSize, 7.0f, false, CAnimator::EDirection::FORWARD);

	m_pAnimatorDying->SetAnimationEndCallback(std::bind(&CGoombas::OnDyingAnimationEnd, this));

	ActivateAnimator(m_pAnimatorWalking);

	m_pTexture->SetSize({ frameSize.x * m_Scale, frameSize.y * m_Scale });
	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());

	m_Rectangle = { position.x, position.y, frameSize.x * m_Scale, frameSize.y * m_Scale };

	m_HorizontalCollider = { m_Rectangle.x + m_HorizontalColliderOffset.x,	m_Rectangle.y + m_HorizontalColliderOffset.y,	15.0f * m_Scale, 9.0f * m_Scale };
	m_VerticalCollider = { m_Rectangle.x + m_VerticalColliderOffset.x,		m_Rectangle.y + m_VerticalColliderOffset.y,		10.0f * m_Scale, 24.0f * m_Scale };

	m_Collider = { m_VerticalCollider.x, m_VerticalCollider.y, m_VerticalCollider.w, m_VerticalCollider.h };

	return true;
}

void CGoombas::Destroy(void)
{

#define DELETE_ANIMATOR(Animator) delete Animator; Animator = nullptr;

	DELETE_ANIMATOR(m_pAnimatorDying);
	DELETE_ANIMATOR(m_pAnimatorWalking);

#undef DELETE_ANIMATOR

	m_pCurrentAnimator = nullptr;

	CGameObject::Destroy();
}

void CGoombas::Kill(void)
{

}

void CGoombas::RenderDebug(void)
{
	CGameObject::RenderDebug();

	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 255, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_HorizontalCollider);

	SDL_SetRenderDrawColor(renderer, 0, 200, 200, 255);
	SDL_RenderDrawRectF(renderer, &m_VerticalCollider);
}

void CGoombas::Update(const float deltaTime)
{
	const SDL_FPoint windowSize = m_pApplication->GetWindowSize();

	if (!m_IsDead)
	{
		const float acceleration = m_EnemyWalkSpeed * deltaTime;

		if (m_HorizontalDirection == EMovementState::MOVING_LEFT)
		{
			m_Velocity.x = std::max(m_Velocity.x - acceleration, -m_MaxWalkingVelocity);
		}
		else if (m_HorizontalDirection == EMovementState::MOVING_RIGHT)
		{
			m_Velocity.x = std::min(m_Velocity.x + acceleration, m_MaxWalkingVelocity);
		}

		m_Rectangle.x += m_Velocity.x * deltaTime;

		if (m_HorizontalDirection == EMovementState::MOVING_LEFT)
		{
			m_pTexture->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
			m_FlipMethod = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
		}
		else if (m_HorizontalDirection == EMovementState::MOVING_RIGHT)
		{
			m_pTexture->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_NONE);
			m_FlipMethod = SDL_RendererFlip::SDL_FLIP_NONE;
		}

		SyncCollider();
	}

	if ((m_Rectangle.x + (m_Rectangle.w * 0.5f)) < 0.0f)
	{
		m_Rectangle.x = windowSize.x - (m_Rectangle.w * 0.5f);
	}
	else if ((m_Rectangle.x - (m_Rectangle.w * 0.5f)) > windowSize.x)
	{
		m_Rectangle.x = -(m_Rectangle.w * 0.5f);
	}

	if (m_pCurrentAnimator)
	{
		m_pCurrentAnimator->Update(deltaTime);
	}
}

void CGoombas::HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime)
{
	for (CGameObject* obstacle : obstacles)
	{
		if (ResolveObstacleXCollision(obstacle->GetCollider()))
		{
			break;
		}
	}
}

void CGoombas::Activate(const SDL_FPoint& spawnPosition, const uint32_t index)
{
	ActivateAnimator(m_pAnimatorWalking);

	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());

	m_Rectangle.x = spawnPosition.x;
	m_Rectangle.y = spawnPosition.y;

	SyncCollider();

	m_StartPosition = spawnPosition;

	m_Index = index;

	m_IsActive = true;
	m_IsDead = false;

	m_Velocity.x = m_MaxWalkingVelocity;

	m_State = EState::ACTIVE;
}

bool CGoombas::ResolveObstacleXCollision(const SDL_FRect& collider)
{
	bool hasCollided = false;

	SDL_FRect intersection = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (QuadVsQuad(m_Collider, collider, &intersection))
	{
		m_Velocity.x = -m_Velocity.x;
		m_HorizontalDirection = (m_HorizontalDirection == EMovementState::MOVING_LEFT)
			? EMovementState::MOVING_RIGHT
			: EMovementState::MOVING_LEFT;

		if (m_HorizontalDirection == EMovementState::MOVING_RIGHT)
		{
			m_pTexture->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_NONE);
			m_FlipMethod = SDL_RendererFlip::SDL_FLIP_NONE;
		}
		else
		{
			m_pTexture->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
			m_FlipMethod = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
		}

		if (intersection.w > 0.0f)
		{
			m_Rectangle.x -= intersection.w * (m_HorizontalDirection == EMovementState::MOVING_LEFT ? 1 : -1);
		}

		SyncCollider();

		hasCollided = true;
	}

	return hasCollided;
}

void CGoombas::SyncCollider(void)
{
	m_HorizontalCollider.x = m_Rectangle.x + m_HorizontalColliderOffset.x;
	m_HorizontalCollider.y = m_Rectangle.y + m_HorizontalColliderOffset.y;
	m_VerticalCollider.x = m_Rectangle.x + m_VerticalColliderOffset.x;
	m_VerticalCollider.y = m_Rectangle.y + m_VerticalColliderOffset.y;

	m_Collider = { m_VerticalCollider.x, m_VerticalCollider.y , m_VerticalCollider.w, m_VerticalCollider.h };
}

void CGoombas::ActivateAnimator(CAnimator* animator)
{
	if (m_pCurrentAnimator != animator)
	{
		m_pCurrentAnimator = animator;
		m_pCurrentAnimator->Reset();
	}
}

void CGoombas::OnDyingAnimationEnd(void)
{
	if (!m_IsActive)
		return;

	if (m_pDyingCallback)
		m_pDyingCallback(m_Index);

	m_IsActive = false;
}
