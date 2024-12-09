#pragma once

#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "Utilities/Animator.h"


class CGoombas final : public CGameObject
{
public:
	typedef std::function<void(const uint32_t index)> Callback;

public:
	CGoombas(void)												   {}
	CGoombas(CApplication* application) : CGameObject(application) {}
	~CGoombas(void)												   {}

	virtual bool	Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth) override;
	virtual void	Destroy(void);
	virtual void	Kill(void) override;
	virtual void	RenderDebug(void) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime) override;
public:

	void			SetDyingCallback(Callback dyingCallback) { m_pDyingCallback = dyingCallback; }

private:

	bool			ResolveObstacleYCollision(const SDL_FRect& collider);
	void			SyncCollider(void);
	void			ActivateAnimator(CAnimator* animator);
	void			OnDyingAnimationEnd(void);

private:

	enum EState
	{
		ACTIVE,
		DEAD
	};

private:

	Callback			m_pDyingCallback = nullptr;

	CAnimator* m_pAnimatorWalking = nullptr;
	CAnimator* m_pCurrentAnimator = nullptr;
	CAnimator* m_pAnimatorDying = nullptr;

	float				m_Scale = 1.2f;

	bool				m_IsActive = false;

	SDL_FPoint			m_StartPosition = { 0.0f, 0.0f };
	SDL_FPoint			m_Velocity = { 0.0f, 0.0f };
	SDL_FPoint			m_ColliderOffset = { 16.0f * m_Scale, 15.0f * m_Scale };

	SDL_RendererFlip	m_FlipMethod = SDL_RendererFlip::SDL_FLIP_NONE;


};

