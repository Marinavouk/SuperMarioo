#include "Pch.h"
#include "GameState.h"

#include "Application.h"

bool CGameState::OnEnter(void)
{
#if defined(_DEBUG) 
	std::cout << "Entering game state" << std::endl;
#endif

	//Set the clear color(the background color that is shown behind the game background and other objects) of the window
	m_pApplication->GetWindow().SetClearColor({0, 0, 0, 255});

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	CFontHandler& fontHandler = m_pApplication->GetFontHandler();
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();
	const SDL_FPoint	windowCenter = m_pApplication->GetWindowCenter();


	//create background
	m_pBackground = textureHandler.CreateTexture("BackgroundMB.png");
	m_pBackground->SetSize(m_pApplication->GetWindowSize());
	m_pBackground->SetAlphaMod(225);

	return true;
}

void CGameState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting game state" << std::endl;
#endif

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();

	//Destroy texture background
	textureHandler.DestroyTexture(m_pBackground->GetName());
	m_pBackground = nullptr;

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
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 200);

	m_pBackground->Render({ 0.0f, 0.0f });

}

void CGameState::RenderDebug(void)
{
	/**
	* Render the game debug objects here, for example colliders/bounding-boxes etc
	* It's completely optional to use this function
	* This function is called every frame
	*/


}