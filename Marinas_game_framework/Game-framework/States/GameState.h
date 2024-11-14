#pragma once

#include "GameObjects/Button.h"
#include "Utilities/Texture.h"
#include "GameObject.h"
#include "State.h"

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
	typedef std::vector<CGameObject*> GameObjectList;

private:
	CTexture* m_pBackground = nullptr;
	CTexture* m_pSuperMario = nullptr;
	CTexture* m_pBricks2 = nullptr;
	CTexture* m_pBricks5 = nullptr;

	TTF_Font* m_pTextFont = nullptr;

	CButton		m_MarioTextBlock = {};
	CButton		m_WorldTextBlock = {};
	CButton		m_TimeTextBlock = {};
	CButton     m_MarioBonusTextBlock = {};
	CButton     m_WorldNumberTextBlock = {};

	CGameObject* m_pPipe = nullptr;


	GameObjectList	m_Obstacles = {};

	float			m_TimerDefault = 246.0f;
	float			m_Timer = m_TimerDefault;


	bool		m_SwitchToGameState = false;
	// Declare the game objects here, create them in the OnEnter function and then finally destroy them in the OnExit function
};