#pragma once

#include "GameObjects/Button.h"
#include "Handlers/TextureHandler.h"
#include "States/State.h"
#include "Utilities/Texture.h"

class CEndOfRoundState final : public CState
{
public:

	CEndOfRoundState(void) {}
	CEndOfRoundState(CApplication* application) : CState(application) {}
	~CEndOfRoundState(void) {}

	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:
	
	TTF_Font* m_pButtonFont = nullptr;
	TTF_Font* m_pGameOverFont = nullptr;


	CTexture* m_pCoinTexture = nullptr;

	CButton		m_GameOverTextBlock = {};
	CButton		m_MarioTextBlock = {};
	CButton		m_LevelTextBlock = {};
	CButton		m_TimeTextBlock = {};


	CButton		m_CoinNumberTextBlock = {};
	CButton		m_ScoreTextBlock = {};
	CButton		m_LevelNumberTextBlock = {};

	float       timer = 0;

};