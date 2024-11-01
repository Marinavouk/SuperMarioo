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

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	CFontHandler& fontHandler = m_pApplication->GetFontHandler();
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();
	const SDL_FPoint	windowCenter = m_pApplication->GetWindowCenter();

	m_pApplication->GetWindow().SetClearColor({200, 200, 200, 255});

	/**
	* Create objects that should be created/started when this state is entered/started (create textures, load/start game music etc)
	* This function is called once, when the game is entering this state
	*/
	m_pBackground = textureHandler.CreateTexture("MainMenuBackGroundSuper-Mario.png");
	m_pBackground->SetSize(m_pApplication->GetWindowSize());
	m_pBackground->SetAlphaMod(225);

	m_pButtonFont = fontHandler.CreateFont("Assets/Fonts/VCR_OSD_MONO.ttf", 60); if (!m_pButtonFont)	return false;

	const SDL_Color titleTextColor = { 200,	0,		0,		255 }; // Dark red
	const SDL_Color buttonBackgroundColor = { 100,	100,	100,	150 }; // Light gray	<-- Background color when the button is not held
	const SDL_Color buttonBackgroundPressedColor = { 100,	100,	100,	200 }; // Dark gray	<-- Background color when the button is held
	const SDL_Color buttonTextColor = { 255, 255,	255,	255 }; // White		<-- Text color when the mouse pointer is outside the button
	const SDL_Color buttonTextColorHovered = { 255,	0,		0,		255 }; // Red		<-- Text color when the mouse pointer is inside (hovering) the button
	const SDL_Color buttonTextColorPressed = { 255,	0,		0,		255 }; // Red		<-- Text color when the button is held


	if (!m_1PlayerGame.Create(m_pApplication, m_pButtonFont, "1 PLAYER GAME", buttonTextColor))
		return false;
	m_1PlayerGame.SetPosition({ windowCenter.x, windowCenter.y + 100.0f });
	
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

	m_1PlayerGame.Destroy(m_pApplication);

	fontHandler.DestroyFont(m_pButtonFont);
	fontHandler.DestroyFont(m_pTextFont);
	m_pButtonFont	= nullptr;
	m_pTextFont		= nullptr;

	textureHandler.DestroyTexture(m_pBackground->GetName());
	
	m_pBackground = nullptr;
}

void CMainMenuState::Update(const float deltaTime)
{
		 if (m_pApplication->GetInputHandler().KeyPressed(SDL_SCANCODE_RETURN)) m_pApplication->SetState(CApplication::EState::GAME);
	else if (m_pApplication->GetInputHandler().KeyPressed(SDL_SCANCODE_ESCAPE)) m_pApplication->SetState(CApplication::EState::QUIT);

		 // Easy access to the input handler and the transition renderer, so you don't have to write m_pApplication->GetX() multiple times below
		// CInputHandler& inputHandler = m_pApplication->GetInputHandler();
		 //const CTransitionRenderer& transitionRenderer = m_pApplication->GetTransitionRenderer();

		 // Update the main-menu objects here

		// m_1PlayerGame.Update(inputHandler);


		 // Will only fade in/out the menu music if the game isn't switching to/from the settings state
		 //if (transitionRenderer.IsTransitioning() && (m_pApplication->GetNextState() != CApplication::EState::SETTINGS) && (m_pApplication->GetLastState() != CApplication::EState::SETTINGS))
			// m_pApplication->GetAudioHandler().SetMusicVolume(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * transitionRenderer.GetTransitionValue()));

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

	m_pBackground->Render({ 0.0f, 0.0f });

	m_1PlayerGame.Render(renderer, &mousePosition);
}

void CMainMenuState::RenderDebug(void)
{
	/**
	* Render the menu debug objects here, for example colliders/bounding-boxes etc
	* It's completely optional to use this function
	* This function is called every frame
	*/


}