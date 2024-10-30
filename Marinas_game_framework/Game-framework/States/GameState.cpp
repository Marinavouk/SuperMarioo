#include "Pch.h"
#include "GameState.h"

#include "Application.h"

bool CGameState::OnEnter(void)
{
#if defined(_DEBUG) 
	std::cout << "Entering game state" << std::endl;
#endif

	/**
	* Set the clear color (the background color that is shown behind the game background and other objects) of the window
	* This is completely optional
	*/
	m_pApplication->GetWindow().SetClearColor({200, 200, 200, 255});

	/**
	* Create objects that should be created/started when this state is entered/started (create textures, load/start game music etc)
	* This function is called once, when the game is entering this state
	*/



	return true;
}

void CGameState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting game state" << std::endl;
#endif

	/**
	* Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures, unload/stop game music etc)
	* This function is called once, when the game is leaving this state
	*/


}

void CGameState::Update(const float deltaTime)
{
	if (m_pApplication->GetInputHandler().KeyPressed(SDL_SCANCODE_ESCAPE))
		m_pApplication->SetState(CApplication::EState::MAIN_MENU);

	/**
	* Update the game objects here
	* This function is called every frame
	*/


}

void CGameState::Render(void)
{
	/**
	* Render the game objects here
	* This function is called every frame
	*/


}

void CGameState::RenderDebug(void)
{
	/**
	* Render the game debug objects here, for example colliders/bounding-boxes etc
	* It's completely optional to use this function
	* This function is called every frame
	*/


}