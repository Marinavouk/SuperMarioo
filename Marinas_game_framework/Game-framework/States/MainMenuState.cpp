#include "Pch.h"
#include "MainMenuState.h"

#include "Application.h"
#include "Globals.h"

bool CMainMenuState::OnEnter(void)
{
#if defined(_DEBUG)
	std::cout << "Entering menu state" << std::endl;
#endif

	m_pApplication->GetWindow().SetClearColor({ 173, 216, 230, 255 });

	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	CFontHandler& fontHandler = m_pApplication->GetFontHandler();
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();
	const SDL_FPoint windowCenter = m_pApplication->GetWindowCenter();

	m_pBackground = textureHandler.CreateTexture("menu.png");
	m_pBackground->SetSize(m_pApplication->GetWindowSize());
	m_pBackground->SetAlphaMod(225);

	m_pCoinTexture = textureHandler.CreateTexture("CoinIdle.png");
	m_pCoinTexture->SetSize({ 24.f, 24.f });

	m_pButtonFont = fontHandler.CreateFont("Assets/Fonts/VCR_OSD_MONO.ttf", 50); if (!m_pButtonFont)	return false;

	const SDL_Color buttonBackgroundColor = { 0,		0,		0,		0 };	// nothing		<-- Background color when the button is not held
	const SDL_Color buttonBackgroundPressedColor = { 0,		0,		0,		0 };	// nothing		<-- Background color when the button is held
	const SDL_Color buttonTextColor = { 255,	255,	255,	255 };	// White		<-- Text color when the mouse pointer is outside the button
	const SDL_Color buttonTextColorHovered = { 100,	100,	100,	255 };	// Light gray	<-- Text color when the mouse pointer is inside (hovering) the button
	const SDL_Color buttonTextColorPressed = { 100,	100,	100,	200 };	// Dark gray	<-- Text color when the button is held

	if (!m_1PlayerGame.Create(m_pApplication, m_pButtonFont, "1 PLAYER GAME", buttonTextColor))
		return false;
	m_1PlayerGame.SetPosition({ windowCenter.x, windowCenter.y + 50.0f });
	m_1PlayerGame.SetBackgroundColor(buttonBackgroundColor);
	m_1PlayerGame.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_1PlayerGame.SetTextColorHovered(buttonTextColorHovered);
	m_1PlayerGame.SetTextColorPressed(buttonTextColorPressed);

	if (!m_2PlayerGame.Create(m_pApplication, m_pButtonFont, "2 PLAYER GAME", buttonTextColor))
		return false;
	m_2PlayerGame.SetPosition({ windowCenter.x, windowCenter.y + 120.0f });
	m_2PlayerGame.SetBackgroundColor(buttonBackgroundColor);
	m_2PlayerGame.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_2PlayerGame.SetTextColorHovered(buttonTextColorHovered);
	m_2PlayerGame.SetTextColorPressed(buttonTextColorPressed);

	e_pMusic = audioHandler.CreateMusic("Assets/Audio/MainMenuTheme.mp3");
	if (!e_pMusic)
		return false;

	audioHandler.PlayMusic(e_pMusic, -1);
	audioHandler.SetMusicVolume(15);

	return true;
}

void CMainMenuState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting menu state" << std::endl;
#endif

	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();

	if (m_pApplication->GetNextState() == CApplication::EState::GAME)
	{
		CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();

		audioHandler.StopMusic();
		audioHandler.DestroyMusic(e_pMusic);
		e_pMusic = nullptr;
	}

	m_2PlayerGame.Destroy(m_pApplication);
	m_1PlayerGame.Destroy(m_pApplication);

	m_pApplication->GetFontHandler().DestroyFont(m_pButtonFont);
	m_pButtonFont = nullptr;

	textureHandler.DestroyTexture(m_pBackground->GetName());
	textureHandler.DestroyTexture(m_pCoinTexture->GetName());
	m_pCoinTexture = nullptr;
	m_pBackground = nullptr;
}

void CMainMenuState::Update(const float deltaTime)
{
	CInputHandler& inputHandler = m_pApplication->GetInputHandler();

	if (inputHandler.KeyPressed(SDL_SCANCODE_RETURN)) m_pApplication->SetState(CApplication::EState::GAME);
	else if (inputHandler.KeyPressed(SDL_SCANCODE_ESCAPE)) m_pApplication->SetState(CApplication::EState::QUIT);

	m_1PlayerGame.Update(inputHandler);
	m_2PlayerGame.Update(inputHandler);

	m_CoinToggleTimer += deltaTime;
	if (m_CoinToggleTimer >= 1.0f)
	{
		m_CoinToggleTimer = 0.0f;
		m_CoinVisible = !m_CoinVisible;
	}
}

void CMainMenuState::Render(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();
	const SDL_FPoint mousePosition = m_pApplication->GetInputHandler().GetMousePosition();

	m_pBackground->Render({ 0.0f, 0.0f });

	if (m_CoinVisible)
		m_pCoinTexture->Render({ 10.0f, 200.0f });

	m_1PlayerGame.Render(renderer, &mousePosition);
	m_2PlayerGame.Render(renderer, &mousePosition);
}

void CMainMenuState::RenderDebug(void)
{

}