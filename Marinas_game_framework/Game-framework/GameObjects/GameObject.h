#pragma once

#include "Utilities/Texture.h"

class CApplication;

class CGameObject
{
public:

	typedef std::vector<CGameObject*> GameObjectList;

public:

	CGameObject(void) {}
	CGameObject(CApplication* application) : m_pApplication(application) {}
	virtual ~CGameObject(void) {}

	virtual bool		Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth);
	virtual void		Destroy(void);
	virtual void		Kill(void);
	virtual void		SetPosition(const SDL_FPoint& position);
	virtual void		Render(void);
	virtual void		RenderDebug(void);
	virtual void		Update(const float deltaTime) {}
	virtual void		HandleInput(const float deltaTime) {}
	virtual void		HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime) {}
	virtual void		HandleEnemyCollision(const GameObjectList& enemies, const float deltaTime) {}
	virtual void		HandleTilemapCollision(const std::vector<SDL_FRect>& tilemapColliders) {}

public:

	SDL_FPoint			GetRectanglePosition(void) const { return { m_Rectangle.x, m_Rectangle.y }; }
	SDL_FPoint			GetRectangleCenterPosition(void) const { return { m_Rectangle.x + (m_Rectangle.w * 0.5f), m_Rectangle.y + (m_Rectangle.h * 0.5f) }; }
	SDL_FPoint			GetRectangleSize(void) const { return { m_Rectangle.w, m_Rectangle.h }; }

	SDL_FPoint			GetColliderPosition(void) const { return { m_Collider.x, m_Collider.y }; }
	SDL_FPoint			GetColliderCenterPosition(void) const { return { m_Collider.x + (m_Collider.w * 0.5f), m_Collider.y + (m_Collider.h * 0.5f) }; }
	SDL_FPoint			GetColliderSize(void) const { return { m_Collider.w, m_Collider.h }; }

	SDL_FRect& GetRectangle(void) const { return (SDL_FRect&)m_Rectangle; }
	SDL_FRect& GetCollider(void) const { return (SDL_FRect&)m_Collider; }

	uint32_t			GetMaxHealth(void) const { return m_MaxHealth; }
	uint32_t			GetCurrentHealth(void) const { return m_CurrentHealth; }

	bool				GetIsDead(void) const { return m_IsDead; }

	SDL_RendererFlip	GetFlipMethod(void) const { return m_FlipMethod; }
	
protected:

	CApplication* m_pApplication = nullptr;

	CTexture*	  m_pTexture = nullptr;

	SDL_RendererFlip	m_FlipMethod = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;

	SDL_FRect		m_Rectangle = { 0.0f, 0.0f, 0.0f, 0.0f };
	SDL_FRect		m_Collider = { 0.0f, 0.0f, 0.0f, 0.0f };

	uint32_t		m_MaxHealth = 0;
	uint32_t		m_CurrentHealth = 0;

	bool			m_IsDead = false;

};