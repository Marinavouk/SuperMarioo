#include "Pch.h"
#include "Pch.h"
#include "EndOfRoundState.h"

#include "Application.h"
#include "Globals.h"

bool CEndOfRoundState::OnEnter(void)
{
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	CFontHandler& fontHandler = m_pApplication->GetFontHandler();
	const SDL_FPoint	windowCenter = m_pApplication->GetWindowCenter();

	m_pApplication->GetWindow().SetClearColor({ 255, 255, 255, 255 });

	const SDL_Color buttonBackgroundColor = { 0,		0,		0,		0 };	// nothing		<-- Background color when the button is not held
	const SDL_Color buttonBackgroundPressedColor = { 0,		0,		0,		0 };	// nothing		<-- Background color when the button is held
	const SDL_Color buttonTextColor = { 255,	255,	255,	255 };	// White		<-- Text color when the mouse pointer is outside the button

	m_pCoinTexture = textureHandler.CreateTexture("CoinIdle.png");
	m_pCoinTexture->SetSize({ 10.f, 16.f });

	m_pButtonFont   = fontHandler.CreateFont("Assets/Fonts/VCR_OSD_MONO.ttf", 25); if (!m_pButtonFont)		return false;
	m_pGameOverFont = fontHandler.CreateFont("Assets/Fonts/VCR_OSD_MONO.ttf", 50); if (!m_pGameOverFont)		return false;

	if (!m_GameOverTextBlock.Create(m_pApplication, m_pButtonFont, "1 PLAYER GAME", buttonTextColor))
		return false;
	m_GameOverTextBlock.SetPosition({ windowCenter.x, windowCenter.y });
	m_GameOverTextBlock.SetBackgroundColor(buttonBackgroundColor);

	return true;
}

void CEndOfRoundState::OnExit(void)
{
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();

	m_GameOverTextBlock.Destroy(m_pApplication);

	textureHandler.DestroyTexture(m_pCoinTexture->GetName());
	m_pCoinTexture = nullptr;

	m_pApplication->GetFontHandler().DestroyFont(m_pGameOverFont);
	m_pGameOverFont = nullptr;

	m_pApplication->GetFontHandler().DestroyFont(m_pButtonFont);
	m_pButtonFont = nullptr;
}

void CEndOfRoundState::Update(const float deltaTime)
{
	const CTransitionRenderer& transitionRenderer = m_pApplication->GetTransitionRenderer();
	
	if (transitionRenderer.IsTransitioning())
		m_pApplication->GetAudioHandler().SetMusicVolume(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * transitionRenderer.GetTransitionValue()));
}

void CEndOfRoundState::Render(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	m_pCoinTexture->Render({ 208.f, 20.0f });
	m_GameOverTextBlock.Render(renderer,nullptr);
}
