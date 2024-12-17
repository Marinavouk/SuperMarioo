#include "Pch.h"
#include "Application.h"

// The different states for the game
#include "States/EndOfRoundState.h"
#include "States/GameState.h"
#include "States/MainMenuState.h"
#include "States/QuitState.h"

bool CApplication::Create(void)
{
	// Creates all the libraries for you, for example SDL, SDL_Image etc
	if (!m_LibraryHandler.Create(this))
		return false;

	m_pRandomNumberGenerator = new CRandom;
	m_pRandomNumberGenerator->Seed();

	if (!m_Window.Create(this, "Super Mario", false, false))
		return false;

	// If you want to set the title of the window while the game is running, you can use this function
//	m_Window.SetTitle("This is the window's new title");

	// If you want to set the color that the renderer's (the big 'screen texture') is "cleared" to, you can use this function
//	m_Window.SetClearColor({255, 0, 0, 255});

	m_TextureHandler = CTextureHandler(this);
	m_FontHandler = CFontHandler(this);
	m_AudioHandler = CAudioHandler(this);
	m_InputHandler = CInputHandler(this);

	if (!m_TextureHandler.Create("Assets/Textures"))
		return false;

	m_pRenderTarget = m_TextureHandler.CreateEmptyTexture({ 512, 416 }, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, "Render target");

	m_TransitionRenderer = CTransitionRenderer(this, m_pRenderTarget->GetSize());

	// If you want to set the speed of the state transition, you can use this function
	// The lower the value is set to, the slower the transition effect will be
	// NOTE: if a value lower than 0.1f is specified, the speed will be clamped to 0.1f
	m_TransitionRenderer.SetSpeed(2.0f);

	/**
	* Create the various states for the game.
	* Here you can add more states if wanted, for example a Settings menu, a Guide menu that explains the game, a Credits menu showing who made the game etc
	*/

	m_pStates[EState::MAIN_MENU] = new CMainMenuState(this);
	m_pStates[EState::GAME] = new CGameState(this);
	m_pStates[EState::QUIT] = new CQuitState(this);
	m_pStates[EState::END_OF_ROUND] = new CEndOfRoundState(this);

	// Set the start state of the game
	m_NextState = EState::MAIN_MENU;
	m_LastState = m_NextState;

	m_pCurrentState = m_pStates[m_NextState];
	if (!m_pCurrentState->OnEnter())
		return false;

	return true;
}

void CApplication::Destroy(void)
{
	if (m_pCurrentState)
		m_pCurrentState->OnExit();

	for (uint32_t i = 0; i < EState::NUM_STATES; ++i)
	{
		if (m_pStates[i])
		{
			delete m_pStates[i];
			m_pStates[i] = nullptr;
		}
	}

	m_TextureHandler.DestroyTexture(m_pRenderTarget->GetName());
	m_pRenderTarget = nullptr;

	m_TextureHandler.Destroy();
	m_Window.Destroy();
	m_LibraryHandler.Destroy();

	delete m_pRandomNumberGenerator;
	m_pRandomNumberGenerator = nullptr;
}

void CApplication::Run(void)
{
	// Main loop - loops as long as the application/game is running
	while (m_Running)
	{
		HandleEvents();
		Update();
		Render();
	}
}

void CApplication::HandleEvents(void)
{
	SDL_Event event = {};
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
		{
			SetState(EState::QUIT);

			break;
		}
		default:
			break;
		}
	}
}

void CApplication::Update(void)
{
	m_InputHandler.Update();
	m_Timer.Update();

	if (m_InputHandler.KeyPressed(SDL_SCANCODE_F1))
		m_DebugRendering = !m_DebugRendering;

	const float deltaTime = (float)m_Timer.GetDeltaTime();

	if (m_pCurrentState)
		m_pCurrentState->Update(deltaTime);

	m_TransitionRenderer.Update(deltaTime);
}

void CApplication::Render(void)
{
	const SDL_Color clearColor = m_Window.GetClearColor();
	m_Window.SetRenderTarget(m_pRenderTarget);
	m_Window.SetClearColor({ 0, 0, 0, 255 });
	m_Window.ClearBuffer();
	m_Window.SetClearColor(clearColor);

	if (m_pCurrentState)
		m_pCurrentState->Render();

	if (m_DebugRendering)
	{
		if (m_pCurrentState)
			m_pCurrentState->RenderDebug();
	}

	m_TransitionRenderer.Render();

	m_Window.SetRenderTarget(nullptr);

	if (m_Window.BeginRender())
	{
		const SDL_FPoint	windowSize = m_Window.GetSize();
		const SDL_FPoint	windowCenter = m_Window.GetCenter();
		const SDL_FPoint	renderTargetSize = m_pRenderTarget->GetSize();
		const SDL_FRect		renderTargetRectangle = { 0.0f, 0.0f, windowSize.x, windowSize.y };

		m_pRenderTarget->Render({ 0.0f, 0.0f }, &renderTargetRectangle);

		m_Window.EndRender();
	}
}

bool CApplication::SetState(const EState nextState)
{
	/**
	* Make sure that m_pNextState is nullptr before continuing
	* If m_pNextState is not nullptr, it means a state transition is occurring
	*/
	if (m_pNextState)
		return false;

	m_pNextState = m_pStates[nextState];

	m_TransitionRenderer.StartTransition();

	m_LastState = m_NextState;
	m_NextState = nextState;

	return true;
}

void CApplication::OnTransitionOpaque(void)
{
	// If there's a pending state
	if (m_pNextState)
	{
		// Stop/exit the current state (see 'OnExit' function in each state) 
		if (m_pCurrentState)
			m_pCurrentState->OnExit();

		// Do the state change
		m_pCurrentState = m_pNextState;

		// And start/enter the new state (see 'OnEnter' function in each state) 
		if (!m_pCurrentState->OnEnter())
			m_Running = false;

		m_pNextState = nullptr;
	}
}

SDL_FPoint CApplication::GetWindowCenter(void) const
{
	const SDL_FPoint windowSize = m_pRenderTarget->GetSize();

	return { windowSize.x * 0.5f, windowSize.y * 0.5f };
}