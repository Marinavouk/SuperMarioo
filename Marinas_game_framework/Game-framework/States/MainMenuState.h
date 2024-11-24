#pragma once

#include "GameObjects/Button.h"
#include "State.h"
#include "Utilities/Texture.h"

class CMainMenuState final : public CState
{
public:

	CMainMenuState(void) {}
	CMainMenuState(CApplication* application) : CState(application) {}
	~CMainMenuState(void) {}

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

	TTF_Font* m_pButtonFont = nullptr;

	CButton		m_1PlayerGame = {};
	CButton		m_2PlayerGame = {};

	bool		m_SwitchToGameState = false;

	float		m_CoinToggleTimer = 0.0f;
	bool		m_CoinVisible = true;

};