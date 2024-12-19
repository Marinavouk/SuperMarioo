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

	void			SpawnGoomba(void);
	void			OnPlayerJumping(void);
	void			OnPlayerDying(void);
	void			OnPlayerEnteringPipe(void);
	void			OnPlayerExitingPipe(void);
	void			OnGoombaStomped(void);
	void			OnGoombaDead(const int index);

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
	CGameObject*	m_pPipeUpperLeft = nullptr;
	CGameObject*	m_pPipeUpperRight = nullptr;
	CGameObject*	m_pPipeLowerLeft = nullptr;
	CGameObject*	m_pPipeLowerRight = nullptr;

	CTexture*		m_pGoombaGUI = nullptr;

	TTF_Font*		m_pTextFont = nullptr;

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

	float			m_TimerDefault = 180.0f;
	float			m_Timer = m_TimerDefault;

	// When the player has died, the game waits this long (in seconds) before fading out and changing to the end-of-round state
	float			m_DeathFadeDelayDefault = 3.0f;
	float			m_DeathFadeDelay = m_DeathFadeDelayDefault;

	bool			m_DeathFadeout = false;
	bool			m_HurryMusicStarted = false;

	Estate			m_State = Estate::IDLE;

	GameObjectList	m_Pipes = {};
	GameObjectList	m_ActiveGoombas = {};
	GameObjectList	m_GoombaPool = {};

};