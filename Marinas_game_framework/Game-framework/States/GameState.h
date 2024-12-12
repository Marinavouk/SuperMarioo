#pragma once

#include "GameObjects/Button.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Tilemap.h"
#include "State.h"
#include "Utilities/Texture.h"

class CGameState final : public CState
{
public:

	 CGameState(void) {}
	 CGameState(CApplication* application) : CState(application) {}
	~CGameState(void) {}

	virtual bool	OnEnter(void) override;
	virtual void	OnExit(void) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	Render(void) override;
	virtual void	RenderDebug(void) override;

private:

	void			OnPlayerDying(void);

private:

	enum Estate
	{
		IDLE = 0,
		ROUND_STARTED,
		ROUND_ENDED,
	};

	typedef std::vector<CGameObject*> GameObjectList;

private:

	CTilemap*		m_pTilemap = nullptr;

	CGameObject*	m_pGoombas = nullptr;

	CGameObject*	m_pPlayer = nullptr;
	CGameObject*	m_pPipeUpperLeft = nullptr;
	CGameObject*	m_pPipeUpperRight = nullptr;
	CGameObject*	m_pPipeLowerLeft = nullptr;
	CGameObject*	m_pPipeLowerRight = nullptr;

	TTF_Font*		m_pTextFont = nullptr;

	CTexture*		m_pCoinTexture = nullptr;

	CButton			m_MarioTextBlock = {};
	CButton			m_WorldTextBlock = {};
	CButton			m_TimeTextBlock = {};
	CButton			m_WorldNumberTextBlock = {};
	CButton		    m_CoinNumberTextBlock = {};

	float			m_TimerDefault = 3.0f;
	float			m_Timer = m_TimerDefault;

	uint32_t		m_VolumeLimiter = 100;

	bool			m_SwitchToGameState = false;

	bool			m_HurryMusicPlayed  = false;
	bool			m_isDeadSound		= false;

	// When the player has died, the game waits this long before fading out and changing to the end-of-round state
	float			m_DeathFadeDelayDefault = 2.0f;
	float			m_DeathFadeDelay = m_DeathFadeDelayDefault;

	bool			m_DeathFadeout = false;

	Estate			m_State = Estate::IDLE;

	GameObjectList	m_Obstacles = {};
	GameObjectList	m_Enemies = {};

};