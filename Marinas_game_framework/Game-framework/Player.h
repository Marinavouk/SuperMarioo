#pragma once

#include "GameObject.h"
#include "Utilities\Animator.h"

class CPlayer final : public CGameObject
{
public:

	typedef std::function<void()> Callback;

public:

	CPlayer(void) {}
	CPlayer(CApplication* application) : CGameObject(application) {}
	~CPlayer(void) {}

	virtual bool	Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth) override;
	virtual void	Destroy(void) override;
	virtual void	Kill(void) override;
	virtual void	Render(void) override;
	virtual void	RenderDebug(void) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	HandleInput(const float deltaTime) override;
	virtual void	HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime) override;
	virtual void	HandleEnemyCollision(const GameObjectList& enemies, const float deltaTime) override;

public:

	void			SetDyingCallback(Callback dyingCallback) { m_pDyingCallback = dyingCallback; }

private:

	bool			ResolveObstacleXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	bool			ResolveObstacleYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	bool			ResolveEnemyXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	bool			ResolveEnemyYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	void			SyncColliders(void);
	void			ActivateAnimator(CAnimator* animator);

private:

	enum EState
	{
		ALIVE = 0,
		DEAD
	};

	enum EMovementState
	{
		IDLE = -1,
		MOVING_LEFT,
		MOVING_RIGHT,
	};

private:

	Callback	m_pDyingCallback = nullptr;

	CAnimator* m_pAnimatorIdle = nullptr;
	CAnimator* m_pAnimatorWalking = nullptr;
	CAnimator* m_pAnimatorRunning = nullptr;
	CAnimator* m_pAnimatorJumping = nullptr;
	CAnimator* m_pCurrentAnimator = nullptr;
	CAnimator* m_pAnimatorDying = nullptr;

	float		m_AccelerationSpeedWalking = 800.0f;
	float		m_DeaccelerationSpeedWalking = 500.0f;
	float		m_AccelerationSpeedRunning = 1000.0f;
	float		m_DeaccelerationSpeedRunning = 700.0f;
	float		m_MaxWalkingVelocity = 200.0f;
	float		m_MaxRunningVelocity = 400.0f;
	float		m_MaxFallVelocity = 600.0f;
	float		m_JumpStrength = 660.0f;

	float		m_Gravity = 1500.0f;
	float		m_Scale = 2.0f;

	// How long time (in seconds) the damage-cooldown should last
	// During the damage-cooldown, the player can't be hurt by the spiders
	float		m_DamageCooldownTimerDefault = 2.0f;
	float		m_DamageCooldownTimer = m_DamageCooldownTimerDefault;

	int32_t		m_HorizontalDirection = EMovementState::IDLE;
	int32_t		m_VerticalDirection = EMovementState::IDLE;

	SDL_FPoint	m_Velocity = { 0.0f, 0.0f };
	SDL_FPoint	m_HorizontalColliderOffset = { 24.0f * m_Scale, 78.0f * m_Scale };
	SDL_FPoint	m_VerticalColliderOffset = { 28.0f * m_Scale, 64.0f * m_Scale };

	// How much (in pixels) the player should be moved out of- and how much to jump when colliding with a spider
	SDL_FPoint	m_HitStrength = { 50.0f, 300.0f };

	SDL_FRect	m_HorizontalCollider = { 0.0f, 0.0f, 0.0f, 0.0f };
	SDL_FRect	m_VerticalCollider = { 0.0f, 0.0f, 0.0f, 0.0f };

	EState		m_State = EState::ALIVE;

	bool		m_IsRunning = false;
	bool		m_IsJumping = false;
	bool		m_DamageCooldown = false;
	bool		m_Show = true;

};