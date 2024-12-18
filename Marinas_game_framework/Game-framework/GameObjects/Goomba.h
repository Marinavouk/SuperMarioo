#pragma once

#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "Utilities/Animator.h"

class CGoomba final : public CGameObject
{
public:

	typedef std::function<void(CGameObject* enemy)> Callback;

public:

	 CGoomba(void) {}
	 CGoomba(CApplication* application) : CGameObject(application) {}
	~CGoomba(void) {}

	virtual bool	Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth) override;
	virtual void	Destroy(void);
	virtual void	Kill(void) override;
	virtual void	RenderDebug(void) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	HandleTilemapCollision(const CTilemap::TileColliders& tilemapColliders, const float deltaTime) override;
	virtual void	HandlePipeCollision(const GameObjectList& pipes, const float deltaTime) override;

public:

	void			SetDyingCallback(Callback dyingCallback) { m_pDyingCallback = dyingCallback; }

public:

	void			Activate(const SDL_FPoint& spawnPosition, const uint32_t index);


private:

	bool			ResolveXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	bool			ResolveTileYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	void			SyncColliders(void);
	void			ActivateAnimator(CAnimator* animator);
	void			OnDyingAnimationEnd(void);

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
		MOVING_LEFT,
		MOVING_RIGHT
	};

private:

	Callback	m_pDyingCallback = nullptr;

	CAnimator*	m_pAnimatorWalking = nullptr;
	CAnimator*	m_pCurrentAnimator = nullptr;
	CAnimator*	m_pAnimatorDying = nullptr;

	float       m_WalkingVelocity = 80.0f;
	float		m_PipeTraversingVelocity = 50.0f;
	float		m_MaxFallVelocity = 600.0f;
	float		m_Gravity = 1500.0f;
	float		m_Scale = 1.2f;
	float		m_PipeTimeoutDefault = 0.5f; // When a goomba is in a pipe, wait this amount of time (in seconds) before the goomba exits the exit pipe
	float		m_PipeTimeout = m_PipeTimeoutDefault;

	int32_t		m_HorizontalDirection = EMovementState::MOVING_LEFT;

	uint32_t	m_ExitPipeDirection = EMovementState::MOVING_LEFT;

	uint32_t	m_Index = 0;

	SDL_FPoint	m_StartPosition = { 0.0f, 0.0f };

	SDL_FPoint	m_Velocity = { 0.0f, 0.0f };
	SDL_FPoint	m_HorizontalColliderOffset = { 5.0f * m_Scale, 14.0f * m_Scale };
	SDL_FPoint	m_VerticalColliderOffset = { 11.0f * m_Scale, 7.0f * m_Scale };
	SDL_FPoint	m_ExitPipePosition = {0.0f, 0.0f};

	SDL_FRect	m_HorizontalCollider = { 0.0f, 0.0f, 0.0f, 0.0f };
	SDL_FRect	m_VerticalCollider = { 0.0f, 0.0f, 0.0f, 0.0f };

	EState		m_State = EState::NORMAL;

};