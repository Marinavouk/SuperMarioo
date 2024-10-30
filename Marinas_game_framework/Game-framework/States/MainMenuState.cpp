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
	m_pBackground->SetAlphaMod(100);


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

	textureHandler.DestroyTexture(m_pBackground->GetName());
	
	m_pBackground = nullptr;
}

void CMainMenuState::Update(const float deltaTime)
{
		 if (m_pApplication->GetInputHandler().KeyPressed(SDL_SCANCODE_RETURN)) m_pApplication->SetState(CApplication::EState::GAME);
	else if (m_pApplication->GetInputHandler().KeyPressed(SDL_SCANCODE_ESCAPE)) m_pApplication->SetState(CApplication::EState::QUIT);

	/**
	* Render the game objects here
	* This function is called every frame
	*/


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

}

void CMainMenuState::RenderDebug(void)
{
	/**
	* Render the menu debug objects here, for example colliders/bounding-boxes etc
	* It's completely optional to use this function
	* This function is called every frame
	*/


}