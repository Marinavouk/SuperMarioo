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

	void			SpawnGoombas(void);
	void			OnPlayerJumping(void);
	void			OnPlayerDying(void);
	void			OnPlayerEnteringPipe(void);
	void			OnPlayerExitingPipe(void);
	void			OnPlayerEnemyStomp(void);
	void			OnEnemyDead(CGameObject* enemy);

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

	CGameObject*	m_pPlayer = nullptr;
	CGameObject*	m_pGoomba1 = nullptr;
	CGameObject*	m_pPipeUpperLeft = nullptr;
	CGameObject*	m_pPipeUpperRight = nullptr;
	CGameObject*	m_pPipeLowerLeft = nullptr;
	CGameObject*	m_pPipeLowerRight = nullptr;

	CTexture*		m_pCoin = nullptr;

	TTF_Font*		m_pTextFont = nullptr;
	TTF_Font*		m_p200TextFont = nullptr;

	Mix_Music*		m_pMusic = nullptr;
	Mix_Music*		m_pHurryMusic = nullptr;

	Mix_Chunk*		m_pJumpSound = nullptr;
	Mix_Chunk*		m_pPipeSound = nullptr;
	Mix_Chunk*		m_pDeathSound = nullptr;
	Mix_Chunk*		m_pGoombaSound = nullptr;

	CButton			m_MarioTextBlock = {};
	CButton			m_WorldTextBlock = {};
	CButton			m_TimeTextBlock = {};
	CButton			m_WorldNumberTextBlock = {};
	CButton		    m_CoinNumberTextBlock = {};

	float			m_TimerDefault = 300.0f;
	float			m_Timer = m_TimerDefault;

	uint32_t		m_VolumeLimiter = 100;
	uint32_t		m_GoombaCount = 0;

	// When the player has died, the game waits this long (in seconds) before fading out and changing to the end-of-round state
	float			m_DeathFadeDelayDefault = 3.0f;
	float			m_DeathFadeDelay = m_DeathFadeDelayDefault;

	bool			m_DeathFadeout = false;
	bool			m_HurryMusicStarted = false;

	Estate			m_State = Estate::IDLE;

	GameObjectList	m_Pipes = {};
	GameObjectList	m_Enemies = {};
	GameObjectList	m_EnemiesPool = {};

};