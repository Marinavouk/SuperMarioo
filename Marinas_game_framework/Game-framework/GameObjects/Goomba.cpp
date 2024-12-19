#include "Pch.h"
#include "Goomba.h"

#include "Application.h"
#include "Utilities/CollisionUtilities.h"

bool CGoomba::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	const SDL_FPoint frameSize = { 32.0f, 32.0f };

	m_pAnimatorWalking = new CAnimator;
	m_pAnimatorDying = new CAnimator;
	m_pAnimatorWalking->Set(m_pTexture, 2, 0, 1, 0, frameSize, 7.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorDying->Set(m_pTexture, 2, 1, 2, 0, frameSize, 5.0f, false, CAnimator::EDirection::FORWARD);
	m_pAnimatorDying->SetAnimationEndCallback(std::bind(&CGoomba::OnDyingAnimationEnd, this));

	ActivateAnimator(m_pAnimatorWalking);

	m_pTexture->SetSize({ frameSize.x * m_Scale, frameSize.y * m_Scale });
	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());

	m_HorizontalDirection = ((m_pApplication->GetRandomNumberGenerator()->RandomUint(0, 1) == 0) ? EMovementState::MOVING_LEFT : EMovementState::MOVING_RIGHT);

	m_Rectangle = { position.x, position.y, frameSize.x * m_Scale, frameSize.y * m_Scale };

	m_HorizontalCollider = { m_Rectangle.x + m_HorizontalColliderOffset.x,	m_Rectangle.y + m_HorizontalColliderOffset.y,	22.0f * m_Scale, 11.0f * m_Scale };
	m_VerticalCollider = { m_Rectangle.x + m_VerticalColliderOffset.x,		m_Rectangle.y + m_VerticalColliderOffset.y,		11.0f * m_Scale, 25.0f * m_Scale };

	m_Collider = { m_VerticalCollider.x, m_VerticalCollider.y, m_VerticalCollider.w, m_VerticalCollider.h };

	m_IsActive = false;

	return true;
}

void CGoomba::Destroy(void)
{
	m_pDyingCallback = nullptr;

#define DELETE_ANIMATOR(Animator) delete Animator; Animator = nullptr;

	DELETE_ANIMATOR(m_pAnimatorDying);
	DELETE_ANIMATOR(m_pAnimatorWalking);

#undef DELETE_ANIMATOR

	m_pCurrentAnimator = nullptr;

	CGameObject::Destroy();
}

void CGoomba::Kill(void)
{
	m_IsActive = false;

	ActivateAnimator(m_pAnimatorDying);

	m_Velocity.x = 0.0f; 

	m_State = EState::DEAD;
}

void CGoomba::Render(void)
{
	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());
	m_pTexture->SetFlipMethod(m_FlipMethod);

	CGameObject::Render();
}

void CGoomba::RenderDebug(void)
{
	CGameObject::RenderDebug();

	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 255, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_HorizontalCollider);

	SDL_SetRenderDrawColor(renderer, 0, 200, 200, 255);
	SDL_RenderDrawRectF(renderer, &m_VerticalCollider);
}

void CGoomba::Update(const float deltaTime)
{
	if (m_State == EState::NORMAL || m_State == EState::DEAD)
	{
		m_Velocity.y = std::min(m_Velocity.y + m_Gravity * deltaTime, m_MaxFallVelocity);
	}

	m_Rectangle.x += m_Velocity.x * deltaTime;
	m_Rectangle.y += m_Velocity.y * deltaTime;

	SyncColliders();

	if (m_State == EState::NORMAL)
	{
		const SDL_FPoint windowSize = m_pApplication->GetWindowSize();

		// Wrap-around mechanic
		// When a goomba leaves the left side of the window, move the goomba to the right side of the window, and vice versa
		if ((m_Rectangle.x + (m_Rectangle.w * 0.5f)) < 0.0f) m_Rectangle.x = windowSize.x - (m_Rectangle.w * 0.5f);
		else if ((m_Rectangle.x + (m_Rectangle.w * 0.5f)) > windowSize.x) m_Rectangle.x = -(m_Rectangle.w * 0.5f);

		if (m_Rectangle.y > (windowSize.y - m_Rectangle.h) - 32.0f)
		{
			m_Rectangle.y = (windowSize.y - m_Rectangle.h) - 32.0f;

			m_Velocity.y = 0.0f;
		}
	}

	else if (m_State == EState::ENTERING_PIPE)
	{
		bool inPipe = false;

		if (m_HorizontalDirection == EMovementState::MOVING_LEFT)
		{
			if (m_Rectangle.x < 0.0f)
				inPipe = true;
		}

		else if (m_HorizontalDirection == EMovementState::MOVING_RIGHT)
		{
			if ((m_Rectangle.x + m_Rectangle.w) > m_pApplication->GetWindowSize().x)
				inPipe = true;
		}

		if (inPipe)
		{
			const bool exitPipeFacingRight = (m_ExitPipeDirection == EMovementState::MOVING_RIGHT);

			m_pTexture->SetFlipMethod((exitPipeFacingRight ? SDL_RendererFlip::SDL_FLIP_NONE : SDL_RendererFlip::SDL_FLIP_HORIZONTAL));

			m_FlipMethod = m_pTexture->GetFlipMethod();

			m_PipeTimeout = m_PipeTimeoutDefault;

			m_HorizontalDirection = (exitPipeFacingRight ? EMovementState::MOVING_RIGHT : EMovementState::MOVING_LEFT);

			m_Velocity.x = 0.0f;

			m_State = EState::IN_PIPE;

			m_Rectangle.x = m_ExitPipePosition.x;
			m_Rectangle.y = m_ExitPipePosition.y;

			SyncColliders();
		}
	}

	else if (m_State == EState::IN_PIPE)
	{
		m_PipeTimeout -= deltaTime;

		if (m_PipeTimeout <= 0.0f)
		{
			m_Velocity.x = ((m_HorizontalDirection == EMovementState::MOVING_LEFT) ? -m_WalkingVelocity : m_WalkingVelocity);

			m_PipeTimeout = 0.0f;

			m_State = EState::EXITING_PIPE;
		}
	}

	SyncColliders();

	if (m_pCurrentAnimator)
		m_pCurrentAnimator->Update(deltaTime);
}

void CGoomba::HandleTilemapCollision(const CTilemap::TileColliders& tilemapColliders, const float deltaTime)
{
	const SDL_FPoint moveAmount = {m_Velocity.x * deltaTime, m_Velocity.y * deltaTime};

	for (const SDL_FRect& collider : tilemapColliders)
	{
		if (ResolveXCollision(collider, moveAmount))
			break;

		if (ResolveTileYCollision(collider, moveAmount))
			break;
	}
}

void CGoomba::HandlePipeCollision(const GameObjectList& pipes, const float deltaTime)
{
	if ((m_State == EState::DEAD) || (m_State == EState::ENTERING_PIPE))
		return;

	const SDL_FPoint moveAmount = { m_Velocity.x * deltaTime, m_Velocity.y * deltaTime };

	if (m_State == EState::NORMAL)
	{
		for (CGameObject* pipe : pipes)
		{
			if (ResolveXCollision(pipe->GetCollider(), moveAmount))
			{
				CGameObject* exitPipe = pipes[m_pApplication->GetRandomNumberGenerator()->RandomUint(0, (uint32_t)pipes.size() - 1)];
				const SDL_FPoint exitPipePosition = exitPipe->GetRectanglePosition();
				const SDL_FPoint exitPipeSize = exitPipe->GetRectangleSize();
				const bool enterRightFacingPipe = (pipe->GetFlipMethod() == SDL_RendererFlip::SDL_FLIP_NONE);
				const bool exitRightFacingPipe = (exitPipe->GetFlipMethod() == SDL_RendererFlip::SDL_FLIP_NONE);

				m_pTexture->SetFlipMethod((enterRightFacingPipe ? SDL_RendererFlip::SDL_FLIP_HORIZONTAL : SDL_RendererFlip::SDL_FLIP_NONE));

				m_HorizontalDirection = (enterRightFacingPipe ? EMovementState::MOVING_LEFT : EMovementState::MOVING_RIGHT);

				m_ExitPipeDirection = (exitRightFacingPipe ? EMovementState::MOVING_RIGHT : EMovementState::MOVING_LEFT);

				m_ExitPipePosition.x = (exitRightFacingPipe ? (exitPipePosition.x + exitPipeSize.x) - m_Rectangle.w : exitPipePosition.x);
				m_ExitPipePosition.y = (exitPipePosition.y + exitPipeSize.y) - m_Rectangle.h;

				m_Velocity.x = (enterRightFacingPipe ? -m_PipeTraversingVelocity : m_PipeTraversingVelocity);

				m_State = EState::ENTERING_PIPE;

				SyncColliders();

				ActivateAnimator(m_pAnimatorWalking);
			}
		}
	}

	else if (m_State == EState::EXITING_PIPE)
	{
		bool pipeCollision = false;

		for (CGameObject* pipe : pipes)
		{
			SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

			if (QuadVsQuad(m_Rectangle, pipe->GetCollider(), &intersection))
			{
				pipeCollision = true;

				break;
			}
		}

		if (!pipeCollision)
		{
			m_Velocity.x = ((m_HorizontalDirection == EMovementState::MOVING_LEFT) ? -m_WalkingVelocity : m_WalkingVelocity);

			m_State = EState::NORMAL;
		}
	}
}

void CGoomba::Activate(const SDL_FPoint& spawnPosition, const uint32_t index, const bool rightDirection)
{
	ActivateAnimator(m_pAnimatorWalking);

	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());

	m_Rectangle.x = spawnPosition.x;
	m_Rectangle.y = spawnPosition.y;

	SyncColliders();

	m_Index = index;

	m_HorizontalDirection = (rightDirection ? EMovementState::MOVING_RIGHT : EMovementState::MOVING_LEFT);

	m_IsActive = true;
	m_IsDead = false;

	m_Velocity.x = m_PipeTraversingVelocity;

	m_State = EState::EXITING_PIPE;
}

bool CGoomba::ResolveXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollided = false;

	// The goomba is moving to the left
	if (moveAmount.x < 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x += intersection.w;

			m_Velocity.x = -m_Velocity.x;

			hasCollided = true;
		}
	}

	// The goomba is moving to the right
	else if (moveAmount.x > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x -= intersection.w;

			m_Velocity.x = -m_Velocity.x;

			hasCollided = true;
		}
	}

	if (hasCollided)
		SyncColliders();

	return hasCollided;
}

bool CGoomba::ResolveTileYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollided = false;

	// The goomba is moving down
	if (moveAmount.y > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_VerticalCollider, collider, &intersection))
		{
			m_Rectangle.y -= intersection.h;

			m_Velocity.y = 0.0f;

			hasCollided = true;
		}
	}

	if (hasCollided)
		SyncColliders();

	return hasCollided;
}

void CGoomba::SyncColliders(void)
{
	m_HorizontalCollider.x = m_Rectangle.x + m_HorizontalColliderOffset.x;
	m_HorizontalCollider.y = m_Rectangle.y + m_HorizontalColliderOffset.y;
	m_VerticalCollider.x = m_Rectangle.x + m_VerticalColliderOffset.x;
	m_VerticalCollider.y = m_Rectangle.y + m_VerticalColliderOffset.y;

	m_Collider = { m_VerticalCollider.x, m_VerticalCollider.y , m_VerticalCollider.w, m_VerticalCollider.h };
}

void CGoomba::ActivateAnimator(CAnimator* animator)
{
	if (m_pCurrentAnimator != animator)
	{
		m_pCurrentAnimator = animator;
		m_pCurrentAnimator->Reset();
	}
}

void CGoomba::OnDyingAnimationEnd(void)
{
	CGameObject::Kill();

	if (m_pDyingCallback)
		m_pDyingCallback(m_Index);
}