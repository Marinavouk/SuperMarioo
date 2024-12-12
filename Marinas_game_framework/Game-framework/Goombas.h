#pragma once

#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "Utilities/Animator.h"


class CGoombas final : public CGameObject
{
public:

	typedef std::function<void(const uint32_t index)> Callback;

public:

	CGoombas(void) {}
	CGoombas(CApplication* application) : CGameObject(application) {}
	~CGoombas(void) {}	

public:

	virtual bool	Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth) override;
	virtual void	Destroy(void);
	virtual void	Kill(void) override;
	virtual void	RenderDebug(void) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime) override;
public:

	void			Activate(const SDL_FPoint& spawnPosition, const uint32_t index);

public:

	void			SetDyingCallback(Callback dyingCallback) { m_pDyingCallback = dyingCallback; }

	bool			GetIsActive(void) const { return m_IsActive; }
	void			SetIsActive(const bool isActive) { m_IsActive = isActive; }

private:

	bool			ResolveObstacleXCollision(const SDL_FRect& collider);
	void			SyncCollider(void);
	void			ActivateAnimator(CAnimator* animator);
	void			OnDyingAnimationEnd(void);

private:

	enum EState
	{
		ACTIVE = 0,
		DEAD
	};

	enum EMovementState
	{
		MOVING_LEFT,
		MOVING_RIGHT
	};

private:
	Callback			m_pDyingCallback = nullptr;

	CAnimator* m_pAnimatorWalking = nullptr;
	CAnimator* m_pCurrentAnimator = nullptr;
	CAnimator* m_pAnimatorDying = nullptr;

	float				m_Scale = 1.2f;
	float				m_EnemyWalkSpeed = 50.0f;
	float               m_MaxWalkingVelocity = 80.0f;

	bool				m_IsActive = false;

	SDL_FPoint			m_StartPosition = { 0.0f, 0.0f };
	SDL_FPoint			m_Velocity = { 0.0f, 0.0f };
	SDL_FPoint			m_ColliderOffset = { 16.0f * m_Scale, 15.0f * m_Scale };

	SDL_FPoint			m_HorizontalColliderOffset = { 6.0f * m_Scale, 12.0f * m_Scale };
	SDL_FPoint			m_VerticalColliderOffset = { 9.0f * m_Scale, 3.0f * m_Scale };

	int32_t				m_HorizontalDirection = EMovementState::MOVING_RIGHT;

	int32_t				m_Index = 0;

	SDL_RendererFlip	m_FlipMethod = SDL_RendererFlip::SDL_FLIP_NONE;

	SDL_FRect			m_HorizontalCollider = { 0.0f, 0.0f, 0.0f, 0.0f };
	SDL_FRect			m_VerticalCollider = { 0.0f, 0.0f, 0.0f, 0.0f };

	EState				m_State = EState::ACTIVE;
};


