#pragma once

#include "GameObjects/Button.h"
#include "Utilities/Texture.h"
#include "State.h"
#include "Tilemap.h"
#include "GameObject.h"

class CGameState final : public CState
{
public:

	 CGameState(void)												{}
	 CGameState(CApplication* application) : CState(application)	{}
	~CGameState(void)												{}

	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;
	virtual void RenderDebug(void) override;

public:

	// Place getters and setters here, in their own public field 


private:

	void			OnPlayerDying(void);


	enum Estate
	{
		IDLE = 0,
		ROUND_STARTED,
		ROUND_ENDED,
	};

	typedef std::vector<CGameObject*> GameObjectList;

private:
	CTexture* m_pBackground = nullptr;
	TTF_Font* m_pTextFont = nullptr;

	CButton		m_MarioTextBlock = {};
	CButton		m_WorldTextBlock = {};
	CButton		m_TimeTextBlock = {};
	CButton     m_MarioBonusTextBlock = {};
	CButton     m_WorldNumberTextBlock = {};

	CGameObject* m_pPipe = nullptr;
	CGameObject* m_pPlayer = nullptr;
	CTilemap*	 m_pTilemap = nullptr;


	GameObjectList	m_Obstacles = {};

	float			m_TimerDefault = 246.0f;
	float			m_Timer = m_TimerDefault;

	uint32_t		m_VolumeLimiter = 100;

	bool		m_SwitchToGameState = false;
	// Declare the game objects here, create them in the OnEnter function and then finally destroy them in the OnExit function

	// When the player has died, the game waits this long before fading out and changing to the end-of-round state
	float			m_DeathFadeDelayDefault = 2.0f;
	float			m_DeathFadeDelay = m_DeathFadeDelayDefault;

	bool			m_DeathFadeout = false;

	Estate			m_State = Estate::IDLE;
};