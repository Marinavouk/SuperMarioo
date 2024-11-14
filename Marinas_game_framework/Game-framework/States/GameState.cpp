#include "Pch.h"
#include "GameState.h"
#include "Pipe.h"

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
	const SDL_FPoint	windowSize = m_pApplication->GetWindowSize();
	const SDL_FPoint	windowCenter = m_pApplication->GetWindowCenter();


	//create background
	m_pBackground = textureHandler.CreateTexture("undergound.png");
	m_pBackground->SetSize(windowSize);

	m_pBricks2 = textureHandler.CreateTexture("brickSeries2.png");
	m_pBricks2->SetSize({ 300.0f, 150.0f });


	m_pPipe = new CPipe(m_pApplication);
	if (!m_pPipe->Create("greenPipe.png", { 0.0f, 0.0f }, 0)) 
	{
		return false;
	}
	m_pPipe->SetPosition({-30.0f, windowSize.y - m_pPipe->GetRectangleSize().y });

	m_Obstacles.push_back(m_pPipe);

	//Upload Font
	m_pTextFont = fontHandler.CreateFont("Assets/Fonts/VCR_OSD_MONO.ttf", 30); if (!m_pTextFont)	return false;
	//Text color
	const SDL_Color titleTextColor = { 255,	255, 255, 255 }; // White
	
	// Buttons can be used as text blocks too, without mouse interaction
	if (!m_MarioTextBlock.Create(m_pApplication, m_pTextFont, "MARIO", titleTextColor))
		return false;
	m_MarioTextBlock.SetPosition({ 100.0f, 10.0f });
	m_MarioTextBlock.SetBackgroundColor({ 0, 0, 0, 0 }); // Only the text in the text block should be visible, so the background is set to be invisible (alpha = 0)
	
	// Buttons can be used as text blocks too, without mouse interaction
	if (!m_WorldTextBlock.Create(m_pApplication, m_pTextFont, "WORLD", titleTextColor))
		return false;
	m_WorldTextBlock.SetPosition({ windowCenter.x + 100.0f, 10.0f });
	m_WorldTextBlock.SetBackgroundColor({ 0, 0, 0, 0 }); // Only the text in the text block should be visible, so the background is set to be invisible (alpha = 0)
	
	// Buttons can be used as text blocks too, without mouse interaction
	if (!m_TimeTextBlock.Create(m_pApplication, m_pTextFont, "TIME", titleTextColor))
		return false;
	m_TimeTextBlock.SetPosition({ windowCenter.x + 400.0f, 10.0f });
	m_TimeTextBlock.SetBackgroundColor({ 0, 0, 0, 0 }); // Only the text in the text block should be visible, so the background is set to be invisible (alpha = 0)

	// Buttons can be used as text blocks too, without mouse interaction
	if (!m_WorldNumberTextBlock.Create(m_pApplication, m_pTextFont, "0-88", titleTextColor))
		return false;
	m_WorldNumberTextBlock.SetPosition({ windowCenter.x + 100.0f, 35.0f });
	m_WorldNumberTextBlock.SetBackgroundColor({ 0, 0, 0, 0 }); // Only the text in the text block should be visible, so the background is set to be invisible (alpha = 0)

	m_Timer = m_TimerDefault;

	return true;
}

void CGameState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting game state" << std::endl;
#endif

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();

	m_WorldNumberTextBlock.Destroy(m_pApplication);
	m_TimeTextBlock.Destroy(m_pApplication);
	m_WorldTextBlock.Destroy(m_pApplication);
	m_MarioTextBlock.Destroy(m_pApplication);

	m_Obstacles.clear();

	m_pPipe->Destroy();
	delete m_pPipe;
	m_pPipe = nullptr;

	textureHandler.DestroyTexture(m_pBricks2->GetName());
	m_pBricks2 = nullptr;

	//Destroy texture background
	textureHandler.DestroyTexture(m_pBackground->GetName());
	m_pBackground = nullptr;

}

void CGameState::Update(const float deltaTime)
{
	if (m_pApplication->GetInputHandler().KeyPressed(SDL_SCANCODE_ESCAPE))
		m_pApplication->SetState(CApplication::EState::MAIN_MENU);


	m_Timer -= deltaTime;

	if (m_Timer <= 0.0f)
	{
		m_Timer = 0.0f;

		//END ROUND
	}


}

void CGameState::Render(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();
	CFontHandler& fontHandler = m_pApplication->GetFontHandler();
	const SDL_FPoint windowCenter = m_pApplication->GetWindowCenter();

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 200);

	m_pBackground->Render({ 0.0f, 0.0f });
	m_pBricks2->Render({ 0.0f, 0.0f });
	m_pPipe->Render();

	m_MarioTextBlock.Render(renderer);
	m_WorldTextBlock.Render(renderer);
	m_TimeTextBlock.Render(renderer);
	m_WorldNumberTextBlock.Render(renderer);

	fontHandler.RenderText(renderer, m_pTextFont, std::to_string((uint32_t)ceilf(m_Timer)), { windowCenter.x + 375.0f, 25.0f }, { 250, 250, 250, 250 });

}

void CGameState::RenderDebug(void)
{
	/**
	* Render the game debug objects here, for example colliders/bounding-boxes etc
	* It's completely optional to use this function
	* This function is called every frame
	*/
	m_pPipe->RenderDebug();

}