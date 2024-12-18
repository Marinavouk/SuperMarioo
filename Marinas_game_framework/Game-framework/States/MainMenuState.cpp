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

	m_pCoinTexture = textureHandler.CreateTexture("coin.png");
	m_pCoinTexture->SetSize({ 10.f, 16.f });

	m_pButtonFont = fontHandler.CreateFont("Assets/Fonts/VCR_OSD_MONO.ttf", 25);
	if (!m_pButtonFont)
		return false;

	const SDL_Color buttonBackgroundColor = { 0,		0,		0,		0 };		// nothing		<-- Background color when the button is not held
	const SDL_Color buttonBackgroundPressedColor = { 0,		0,		0,		0 };	// nothing		<-- Background color when the button is held
	const SDL_Color buttonTextColor = { 255,	255,	255,	255 };				// White		<-- Text color when the mouse pointer is outside the button
	const SDL_Color buttonTextColorHovered = { 100,	100,	100,	255 };			// Light gray	<-- Text color when the mouse pointer is inside (hovering) the button
	const SDL_Color buttonTextColorPressed = { 100,	100,	100,	200 };			// Dark gray	<-- Text color when the button is held

	if (!m_1PlayerGame.Create(m_pApplication, m_pButtonFont, "1 PLAYER GAME", buttonTextColor))
		return false;
	m_1PlayerGame.SetPosition({ windowCenter.x, windowCenter.y + 40.0f });
	m_1PlayerGame.SetBackgroundColor(buttonBackgroundColor);
	m_1PlayerGame.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_1PlayerGame.SetTextColorHovered(buttonTextColorHovered);
	m_1PlayerGame.SetTextColorPressed(buttonTextColorPressed);

	m_pMusic = audioHandler.CreateMusic("Assets/Audio/menu.mp3");
	if (!m_pMusic)
		return false;

	audioHandler.PlayMusic(m_pMusic, -1);

	return true;
}

void CMainMenuState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting menu state" << std::endl;
#endif

	if (m_pApplication->GetNextState() == CApplication::EState::GAME)
	{
		CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();

		audioHandler.StopMusic();
		audioHandler.DestroyMusic(m_pMusic);
		m_pMusic = nullptr;
	}
	m_1PlayerGame.Destroy(m_pApplication);

	m_pApplication->GetFontHandler().DestroyFont(m_pButtonFont);
	m_pButtonFont = nullptr;

	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();

	textureHandler.DestroyTexture(m_pCoinTexture->GetName());
	textureHandler.DestroyTexture(m_pBackground->GetName());
	m_pCoinTexture = nullptr;
	m_pBackground = nullptr;
}

void CMainMenuState::Update(const float deltaTime)
{
	CInputHandler& inputHandler = m_pApplication->GetInputHandler();

		 if (inputHandler.KeyPressed(SDL_SCANCODE_RETURN)) m_pApplication->SetState(CApplication::EState::GAME);
	else if (inputHandler.KeyPressed(SDL_SCANCODE_ESCAPE)) m_pApplication->SetState(CApplication::EState::QUIT);

	m_1PlayerGame.Update(inputHandler);

	if (m_1PlayerGame.IsPressed(inputHandler)) m_pApplication->SetState(CApplication::EState::GAME);
	
	m_CoinToggleTimer += deltaTime;
	if (m_CoinToggleTimer >= 0.5f)
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
		m_pCoinTexture->Render({ 208.f, 20.0f });

	m_1PlayerGame.Render(renderer, &mousePosition);
}