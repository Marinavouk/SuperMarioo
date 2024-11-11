#pragma once

#include "GameObjects/Button.h"
#include "State.h"
#include "Utilities/Texture.h"

class CMainMenuState final : public CState
{
public:

	 CMainMenuState(void)												{}
	 CMainMenuState(CApplication* application) : CState(application)	{}
	~CMainMenuState(void)												{}

	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;
	virtual void RenderDebug(void) override;

public:

	// Place getters and setters here, in their own public field 

private:
	CTexture* m_pBackground = nullptr;
	
	CTexture* m_pCoinTexture = nullptr;

	TTF_Font* m_pTextFont = nullptr;
	TTF_Font* m_pButtonFont = nullptr;


	CButton		m_1PlayerGame = {};
	CButton		m_2PlayerGame = {};

	SDL_FPoint	m_CoinSize = { 0.0f, 0.0f };
	SDL_FPoint	m_CoinPosition = { 520.0f, 38.0f };

	bool		m_SwitchToGameState = false;
	// Declare the game objects here, create them in the OnEnter function and then finally destroy them in the OnExit function


};