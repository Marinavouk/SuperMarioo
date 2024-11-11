#include "Pch.h"
#include "MainMenuState.h"

#include "Application.h"

bool CMainMenuState::OnEnter(void)
{
#if defined(_DEBUG)
	std::cout << "Entering menu state" << std::endl;
#endif

	/**
	* Set the clear color (the background color that is shown behind the menu background and other objects) of the window
	* This is completely optional
	*/
	m_pApplication->GetWindow().SetClearColor({ 173, 216, 230, 255 }); 

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	CFontHandler& fontHandler = m_pApplication->GetFontHandler();
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();
	const SDL_FPoint	windowCenter = m_pApplication->GetWindowCenter();


	/**
	* Create objects that should be created/started when this state is entered/started (create textures, load/start game music etc)
	* This function is called once, when the game is entering this state
	*/
	m_pBackground = textureHandler.CreateTexture("MainBack.png");
	m_pBackground->SetSize(m_pApplication->GetWindowSize());
	m_pBackground->SetAlphaMod(225);

	m_pCoinTexture = textureHandler.CreateTexture("CoinIdle.png");
	m_pCoinTexture->SetSize({24.f, 24.f});

	m_pButtonFont = fontHandler.CreateFont("Assets/Fonts/VCR_OSD_MONO.ttf", 50); if (!m_pButtonFont)	return false;

	const SDL_Color titleTextColor = { 200,	0,		0,		255 }; // Dark red
	const SDL_Color buttonBackgroundColor = { 0,	0,	0,	0 }; // nothing	<-- Background color when the button is not held
	const SDL_Color buttonBackgroundPressedColor = { 0,	0,	0,	0 }; // nothing	<-- Background color when the button is held
	const SDL_Color buttonTextColor = { 255, 255,	255,	255 }; // White		<-- Text color when the mouse pointer is outside the button
	const SDL_Color buttonTextColorHovered = { 100,	 100,	100,	255 }; // Light gray<-- Text color when the mouse pointer is inside (hovering) the button
	const SDL_Color buttonTextColorPressed = { 100,	100,	100,	200 }; // Dark gray	<-- Text color when the button is held


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

	return true;
}

void CMainMenuState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting menu state" << std::endl;
#endif

	/**
	* Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures, unload/stop menu music etc)
	* This function is called once, when the game is leaving this state
	*/

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	CFontHandler& fontHandler = m_pApplication->GetFontHandler();

	m_2PlayerGame.Destroy(m_pApplication);
	m_1PlayerGame.Destroy(m_pApplication);

	fontHandler.DestroyFont(m_pButtonFont);
	fontHandler.DestroyFont(m_pTextFont);
	m_pButtonFont	= nullptr;
	m_pTextFont		= nullptr;

	
	textureHandler.DestroyTexture(m_pBackground->GetName());
	textureHandler.DestroyTexture(m_pCoinTexture->GetName());
	m_pCoinTexture= nullptr;
	m_pBackground = nullptr;
}

void CMainMenuState::Update(const float deltaTime)
{
		 if (m_pApplication->GetInputHandler().KeyPressed(SDL_SCANCODE_RETURN)) m_pApplication->SetState(CApplication::EState::GAME);
	else if (m_pApplication->GetInputHandler().KeyPressed(SDL_SCANCODE_ESCAPE)) m_pApplication->SetState(CApplication::EState::QUIT);

		 // Easy access to the input handler and the transition renderer, so you don't have to write m_pApplication->GetX() multiple times below
		 CInputHandler& inputHandler = m_pApplication->GetInputHandler();
		 const CTransitionRenderer& transitionRenderer = m_pApplication->GetTransitionRenderer();

		 // Update the main-menu objects here

		 m_1PlayerGame.Update(inputHandler);
		 m_2PlayerGame.Update(inputHandler);

		 // Switch state whenever any of the buttons are pressed
		 if (m_1PlayerGame.IsPressed(inputHandler))		m_pApplication->SetState(CApplication::EState::GAME);
		 else if (m_2PlayerGame.IsPressed(inputHandler))	m_pApplication->SetState(CApplication::EState::GAME);
		
		 

}

void CMainMenuState::Render(void)
{
	/**
	* Render the menu objects here
	* This function is called every frame
	*/
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();
	const SDL_FPoint	mousePosition = m_pApplication->GetInputHandler().GetMousePosition();

	// Render the main-menu objects here

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 200);

	m_pBackground->Render({ 0.0f, 0.0f });
	m_pCoinTexture->Render(m_CoinPosition);
	
	m_1PlayerGame.Render(renderer, &mousePosition);
	m_2PlayerGame.Render(renderer, &mousePosition);

}

void CMainMenuState::RenderDebug(void)
{
	/**
	* Render the menu debug objects here, for example colliders/bounding-boxes etc
	* It's completely optional to use this function
	* This function is called every frame
	*/


}