#pragma once


#include "GameObject.h"
#include "Utilities\Animator.h"
#include "Tilemap.h"


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
	virtual void	RenderDebug(void) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	HandleInput(const float deltaTime) override;
	virtual void	HandleTilemapCollision(const CTilemap::TileColliders& tilemapColliders, const float deltaTime) override;
	virtual void	HandlePipeCollision(const GameObjectList& pipes, const float deltaTime) override;
	virtual void	HandleEnemyCollision(const GameObjectList& enemies, const float deltaTime);


public:


	void			SetJumpingCallback(Callback dyingCallback) { m_pJumpingCallback = dyingCallback; }
	void			SetDyingCallback(Callback dyingCallback) { m_pDyingCallback = dyingCallback; }
	void			SetEnteringPipeCallback(Callback dyingCallback) { m_pEnteringPipeCallback = dyingCallback; }
	void			SetExitingPipeCallback(Callback dyingCallback) { m_pExitingPipeCallback = dyingCallback; }
	void			SetEnemyStompCallback(Callback dyingCallback) { m_pEnemyStompCallback = dyingCallback; }


private:


	bool			ResolveTileYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	bool			ResolveEnemyYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	bool			ResolveXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	void			SyncColliders(void);
	void			ActivateAnimator(CAnimator* animator);


private:


	enum EState
	{
		NORMAL = 0,
		DEAD,
		ENTERING_PIPE,
		EXITING_PIPE,
		IN_PIPE
	};


	enum EMovementState
	{
		IDLE = -1,
		MOVING_LEFT,
		MOVING_RIGHT,
	};


private:


	Callback	m_pJumpingCallback = nullptr;
	Callback	m_pDyingCallback = nullptr;
	Callback	m_pEnteringPipeCallback = nullptr;
	Callback	m_pExitingPipeCallback = nullptr;
	Callback	m_pEnemyStompCallback = nullptr;


	CAnimator*	m_pAnimatorIdle = nullptr;
	CAnimator*	m_pAnimatorWalking = nullptr;
	CAnimator*	m_pAnimatorRunning = nullptr;
	CAnimator*	m_pAnimatorJumping = nullptr;
	CAnimator*	m_pAnimatorDying = nullptr;
	CAnimator*	m_pCurrentAnimator = nullptr;


	float		m_AccelerationSpeedWalking = 800.0f;
	float		m_DeaccelerationSpeedWalking = 500.0f;
	float		m_AccelerationSpeedRunning = 1000.0f;
	float		m_DeaccelerationSpeedRunning = 700.0f;
	float		m_PipeTraversingVelocity = 70.0f;
	float		m_MaxWalkingVelocity = 200.0f;
	float		m_MaxRunningVelocity = 400.0f;
	float		m_MaxFallVelocity = 600.0f;
	float		m_JumpStrength = 600.0f;
	float		m_DyingJumpStrength = 500.0f;
	float		m_EnemyStompJumpStrength = 200.0f;
	float		m_Gravity = 1500.0f;
	float		m_DyingGravity = 200.0f;
	float		m_Scale = 1.6f;
	float		m_PipeTimeoutDefault = 0.5f; // When Mario is in a pipe, wait this amount of time (in seconds) before Mario exits the exit pipe
	float		m_PipeTimeout = m_PipeTimeoutDefault;


	int32_t		m_HorizontalDirection = EMovementState::IDLE;
	int32_t		m_VerticalDirection = EMovementState::IDLE;


	uint32_t	m_ExitPipeDirection = EMovementState::MOVING_LEFT;


	SDL_FPoint	m_Velocity = { 0.0f, 0.0f };
	SDL_FPoint	m_HorizontalColliderOffset = { 6.0f * m_Scale, 10.0f * m_Scale };
	SDL_FPoint	m_VerticalColliderOffset = { 9.0f * m_Scale, 4.0f * m_Scale };
	SDL_FPoint	m_ExitPipePosition = {0.0f, 0.0f};


	SDL_FRect	m_HorizontalCollider = { 0.0f, 0.0f, 0.0f, 0.0f };
	SDL_FRect	m_VerticalCollider = { 0.0f, 0.0f, 0.0f, 0.0f };


	EState		m_State = EState::NORMAL;


	bool		m_OnGround = true;
	bool		m_IsRunning = false;
	bool		m_IsJumping = false;
	bool		m_IsDyingJumping = false; 


};
