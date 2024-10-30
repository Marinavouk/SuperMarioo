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
	m_pApplication->GetWindow().SetClearColor({200, 200, 200, 255});

	/**
	* Create objects that should be created/started when this state is entered/started (create textures, load/start game music etc)
	* This function is called once, when the game is entering this state
	*/



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


}

void CMainMenuState::RenderDebug(void)
{
	/**
	* Render the menu debug objects here, for example colliders/bounding-boxes etc
	* It's completely optional to use this function
	* This function is called every frame
	*/


}