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

	m_pApplication->GetWindow().SetClearColor({ 0, 0, 0, 255 });

	const SDL_Color buttonBackgroundColor = { 0,		0,		0,		0 };	// nothing		<-- Background color when the button is not held
	const SDL_Color buttonBackgroundPressedColor = { 0,		0,		0,		0 };	// nothing		<-- Background color when the button is held
	const SDL_Color buttonTextColor = { 255,	255,	255,	255 };	// WHITE		<-- Text color when the mouse pointer is outside the button

	m_pCoinTexture = textureHandler.CreateTexture("CoinIdle.png");
	m_pCoinTexture->SetSize({ 20.f, 20.f });

	m_pButtonFont   = fontHandler.CreateFont("Assets/Fonts/VCR_OSD_MONO.ttf", 20); if (!m_pButtonFont)		return false;
	m_pGameOverFont = fontHandler.CreateFont("Assets/Fonts/VCR_OSD_MONO.ttf", 50); if (!m_pGameOverFont)    return false;

	if (!m_GameOverTextBlock.Create(m_pApplication, m_pButtonFont, "GAME OVER", buttonTextColor))
		return false;
	m_GameOverTextBlock.SetPosition({ windowCenter.x, windowCenter.y });
	m_GameOverTextBlock.SetBackgroundColor(buttonBackgroundColor);
	
	if (!m_MarioTextBlock.Create(m_pApplication, m_pButtonFont, "MARIO", buttonTextColor))
		return false;
	m_MarioTextBlock.SetPosition({ 50.0f, 10.0f });
	m_MarioTextBlock.SetBackgroundColor(buttonBackgroundColor);
	
	if (!m_WorldTextBlock.Create(m_pApplication, m_pButtonFont, "WORLD", buttonTextColor))
		return false;
	m_WorldTextBlock.SetPosition({ windowCenter.x + 70.0f, 10.0f });
	m_WorldTextBlock.SetBackgroundColor(buttonBackgroundColor);
	
	if (!m_TimeTextBlock.Create(m_pApplication, m_pButtonFont, "TIME", buttonTextColor))
		return false;
	m_TimeTextBlock.SetPosition({ windowCenter.x + 160.0f, 10.0f });
	m_TimeTextBlock.SetBackgroundColor(buttonBackgroundColor);
	
	if (!m_WorldNumberTextBlock.Create(m_pApplication, m_pButtonFont, "0-88", buttonTextColor))
		return false;
	m_WorldNumberTextBlock.SetPosition({ windowCenter.x + 70.0f, 30.0f });
	m_WorldNumberTextBlock.SetBackgroundColor(buttonBackgroundColor);
	
	if (!m_CoinNumberTextBlock.Create(m_pApplication, m_pButtonFont, "x00", buttonTextColor))
		return false;
	m_CoinNumberTextBlock.SetPosition({ 210.0f, 30.0f });
	m_CoinNumberTextBlock.SetBackgroundColor(buttonBackgroundColor);

	return true;
}

void CEndOfRoundState::OnExit(void)
{
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	
	m_CoinNumberTextBlock.Destroy(m_pApplication);
	m_WorldNumberTextBlock.Destroy(m_pApplication);
	m_TimeTextBlock.Destroy(m_pApplication);
	m_WorldTextBlock.Destroy(m_pApplication);
	m_MarioTextBlock.Destroy(m_pApplication);
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
	
	timer += deltaTime;

	if (timer >= 3) 
	{
		m_pApplication->SetState(CApplication::EState::MAIN_MENU);
	}


	if (transitionRenderer.IsTransitioning())
		m_pApplication->GetAudioHandler().SetMusicVolume(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * transitionRenderer.GetTransitionValue()));
}

void CEndOfRoundState::Render(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	m_pCoinTexture->Render({ 175.f, 20.0f });
	m_GameOverTextBlock.Render(renderer,nullptr);
	m_MarioTextBlock.Render(renderer,nullptr);
	m_WorldTextBlock.Render(renderer,nullptr);
	m_TimeTextBlock.Render(renderer,nullptr);
	m_WorldNumberTextBlock.Render(renderer,nullptr);
	m_CoinNumberTextBlock.Render(renderer,nullptr);

}
