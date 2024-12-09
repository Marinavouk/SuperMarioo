#include "Pch.h"
#include "GameState.h"
#include <iomanip>
#include <sstream>

#include "Application.h"
#include "GameObjects/Pipe.h"
#include "GameObjects/Player.h"
#include "Goombas.h"
#include "Globals.h"
#include "Utilities/CollisionUtilities.h"

bool CGameState::OnEnter(void)
{
	m_pApplication->GetWindow().SetClearColor({ 0, 0, 0, 255 });

	const SDL_FPoint windowSize   = m_pApplication->GetWindowSize();
	const SDL_FPoint windowCenter = m_pApplication->GetWindowCenter();
	CAudioHandler&   audioHandler  = m_pApplication->GetAudioHandler();
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();

	m_pTilemap = new CTilemap;
	if (!m_pTilemap->Create(m_pApplication))
		return false;

	const SDL_FPoint tileSize = m_pTilemap->GetTileSize();

	m_pPlayer = new CPlayer(m_pApplication);
	if (!m_pPlayer->Create("SuperMario.png", { 0.0f, 0.0f }, 1))
		return false;
	m_pPlayer->SetPosition({ 250.0f, (windowSize.y - m_pPlayer->GetRectangleSize().y) - tileSize.y });
	CPlayer* player = (CPlayer*)m_pPlayer;
	player->SetDyingCallback(std::bind(&CGameState::OnPlayerDying, this));

	m_pGoombas = new CGoombas(m_pApplication);
	if (!m_pGoombas->Create("goomba.png", { 0.0f, 0.0f }, 1))
		return false;
	m_pGoombas->SetPosition({ 250.0f, (windowSize.y - m_pGoombas->GetRectangleSize().y) - tileSize.y });
	CGoombas* goombas = (CGoombas*)m_pGoombas;

	m_pCoinTexture = textureHandler.CreateTexture("CoinIdle.png");
	m_pCoinTexture->SetSize({ 16.f, 16.f });

	m_pPipeUpperLeft = new CPipe(m_pApplication);
	if (!m_pPipeUpperLeft->Create("pipe.png", { 0.0f, 0.0f }, 0))
		return false;
	m_pPipeUpperLeft->SetPosition({ 0.0f, tileSize.y * 2.0f });
	m_pPipeUpperLeft->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_NONE);

	m_pPipeUpperRight = new CPipe(m_pApplication);
	if (!m_pPipeUpperRight->Create("pipe.png", { 0.0f, 0.0f }, 0))
		return false;
	m_pPipeUpperRight->SetPosition({ windowSize.x - m_pPipeUpperRight->GetRectangleSize().x, tileSize.y * 2.0f });
	m_pPipeUpperRight->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

	m_pPipeLowerLeft = new CPipe(m_pApplication);
	if (!m_pPipeLowerLeft->Create("pipe.png", { 0.0f, 0.0f }, 0))
		return false;
	m_pPipeLowerLeft->SetPosition({ 0.0f, (windowSize.y - m_pPipeLowerLeft->GetRectangleSize().y) - tileSize.y });
	m_pPipeLowerLeft->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_NONE);

	m_pPipeLowerRight = new CPipe(m_pApplication);
	if (!m_pPipeLowerRight->Create("pipe.png", { 0.0f, 0.0f }, 0))
		return false;
	m_pPipeLowerRight->SetPosition({ windowSize.x - m_pPipeLowerRight->GetRectangleSize().x, (windowSize.y - m_pPipeLowerRight->GetRectangleSize().y) - tileSize.y });
	m_pPipeLowerRight->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

	m_Obstacles.push_back(m_pPipeUpperLeft);
	m_Obstacles.push_back(m_pPipeUpperRight);
	m_Obstacles.push_back(m_pPipeLowerRight);
	m_Obstacles.push_back(m_pPipeLowerLeft);

	m_pTextFont = m_pApplication->GetFontHandler().CreateFont("Assets/Fonts/VCR_OSD_MONO.ttf", 18);
	if (!m_pTextFont)
		return false;

	const SDL_Color titleTextColor = { 255,	255, 255, 255 }; // White

	// Buttons can be used as text blocks too, without mouse interaction
	if (!m_MarioTextBlock.Create(m_pApplication, m_pTextFont, "MARIO", titleTextColor))
		return false;
	m_MarioTextBlock.SetPosition({ 100.0f, 10.0f });
	m_MarioTextBlock.SetBackgroundColor({ 0, 0, 0, 0 }); // Only the text in the text block should be visible, so the background is set to be invisible (alpha = 0)

	if (!m_WorldTextBlock.Create(m_pApplication, m_pTextFont, "WORLD", titleTextColor))
		return false;
	m_WorldTextBlock.SetPosition({ windowCenter.x + 50.0f, 10.0f });
	m_WorldTextBlock.SetBackgroundColor({ 0, 0, 0, 0 });

	if (!m_TimeTextBlock.Create(m_pApplication, m_pTextFont, "TIME", titleTextColor))
		return false;
	m_TimeTextBlock.SetPosition({ windowCenter.x + 140.0f, 10.0f });
	m_TimeTextBlock.SetBackgroundColor({ 0, 0, 0, 0 });

	if (!m_WorldNumberTextBlock.Create(m_pApplication, m_pTextFont, "0-88", titleTextColor))
		return false;
	m_WorldNumberTextBlock.SetPosition({ windowCenter.x + 50.0f, 35.0f });
	m_WorldNumberTextBlock.SetBackgroundColor({ 0, 0, 0, 0 });
	
	if (!m_CoinNumberTextBlock.Create(m_pApplication, m_pTextFont, "x00", titleTextColor))
		return false;
	m_CoinNumberTextBlock.SetPosition({ 205.0f, 18.0f });
	m_CoinNumberTextBlock.SetBackgroundColor({ 0, 0, 0, 0 });

	e_pMusic = audioHandler.CreateMusic("Assets/Audio/Underground.mp3");
	if (!e_pMusic)
		return false;

	e_pHurryMusic = audioHandler.CreateMusic("Assets/Audio/Underground (Hurry!).mp3");
	if (!e_pHurryMusic)
		return false;

	e_pDeathSound = audioHandler.CreateMusic("Assets/Audio/Mario_Death.mp3");
	if (!e_pDeathSound)
		return false;

	audioHandler.PlayMusic(e_pMusic, -1);
	audioHandler.SetMusicVolume(15);


	m_Timer = m_TimerDefault;

	m_DeathFadeout = false;

	m_State = Estate::IDLE;

	e_EndOfRoundPlayerKilled = false;

	return true;
}

void CGameState::OnExit(void)
{
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();

	if (e_pDeathSound || m_pApplication->GetNextState() == CApplication::EState::QUIT)
	{
		audioHandler.DestroyMusic(e_pDeathSound);
		e_pDeathSound = nullptr;
	}
	
	if (e_pHurryMusic || m_pApplication->GetNextState() == CApplication::EState::QUIT)
	{
		audioHandler.DestroyMusic(e_pHurryMusic);
		e_pHurryMusic = nullptr;
	}

	if (m_pApplication->GetNextState() == CApplication::EState::QUIT)
	{
		audioHandler.StopMusic();
		audioHandler.DestroyMusic(e_pMusic);
		e_pMusic = nullptr;
	}
	
	m_CoinNumberTextBlock.Destroy(m_pApplication);
	m_WorldNumberTextBlock.Destroy(m_pApplication);
	m_TimeTextBlock.Destroy(m_pApplication);
	m_WorldTextBlock.Destroy(m_pApplication);
	m_MarioTextBlock.Destroy(m_pApplication);

	m_pApplication->GetFontHandler().DestroyFont(m_pTextFont);
	m_pTextFont = nullptr;

	m_pPipeUpperLeft->Destroy();
	delete m_pPipeUpperLeft;
	m_pPipeUpperLeft = nullptr;

	m_pPipeUpperRight->Destroy();
	delete m_pPipeUpperRight;
	m_pPipeUpperRight = nullptr;

	m_pPipeLowerRight->Destroy();
	delete m_pPipeLowerRight;
	m_pPipeLowerRight = nullptr;

	m_pPipeLowerLeft->Destroy();
	delete m_pPipeLowerLeft;
	m_pPipeLowerLeft = nullptr;

	textureHandler.DestroyTexture(m_pCoinTexture->GetName());
	m_pCoinTexture = nullptr;

	m_pGoombas->Destroy();
	delete m_pGoombas;
	m_pGoombas = nullptr;

	m_pPlayer->Destroy();
	delete m_pPlayer;
	m_pPlayer = nullptr;

	m_pTilemap->Destroy();
	delete m_pTilemap;
	m_pTilemap = nullptr;

	m_Obstacles.clear();
}

void CGameState::Update(const float deltaTime)
{
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();

	if (m_pApplication->GetInputHandler().KeyPressed(SDL_SCANCODE_ESCAPE))
		m_pApplication->SetState(CApplication::EState::QUIT);

	if (m_State == Estate::IDLE)
	{
		if (!m_pApplication->GetTransitionRenderer().IsTransitioning())
			m_State = Estate::ROUND_STARTED;
	}

	else if (m_State == Estate::ROUND_STARTED)
	{
		// The round has started, so update the game objects here so they can move etc

		m_pPlayer->HandleInput(deltaTime);
		m_pPlayer->Update(deltaTime);
		m_pPlayer->HandleTilemapCollision(m_pTilemap->GetColliders(), deltaTime);
		m_pPlayer->HandleObstacleCollision(m_Obstacles, deltaTime);

		m_pGoombas->Update(deltaTime);

		//m_Timer -= deltaTime;

		if (m_Timer <= 60.0f && !m_HurryMusicPlayed && m_Timer >= 1.0f)
		{
			audioHandler.StopMusic();
			audioHandler.PlayMusic(e_pHurryMusic, -1);
			audioHandler.SetMusicVolume(15);
			m_HurryMusicPlayed = true;
		}

		else if (m_Timer <= 0.0f)
		{		
			m_Timer = 0.0f;

			m_State = Estate::ROUND_ENDED;

			m_HurryMusicPlayed = false;

			e_EndOfRoundPlayerKilled = false;

			m_pApplication->SetState(CApplication::EState::END_OF_ROUND);
		}
	}

	else if (m_State == Estate::ROUND_ENDED)
	{
		m_pPlayer->Update(deltaTime);
		m_pPlayer->HandleObstacleCollision(m_Obstacles, deltaTime);
		if (m_DeathFadeout)
		{
			m_DeathFadeDelay -= deltaTime;

			if (m_DeathFadeDelay <= 0.0f)
			{
				m_DeathFadeDelay = 0.0f;

				m_DeathFadeout = false;

				m_pApplication->SetState(CApplication::EState::END_OF_ROUND);
			}
		}
	}
}

void CGameState::Render(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();
	CFontHandler& fontHandler = m_pApplication->GetFontHandler();

	m_pTilemap->Render();
	m_pPlayer->Render();
	m_pGoombas->Render();
	m_pCoinTexture->Render({ 175.f, 10.0f });
	m_pPipeUpperLeft->Render();
	m_pPipeUpperRight->Render();
	m_pPipeLowerLeft->Render();
	m_pPipeLowerRight->Render();

	m_MarioTextBlock.Render(renderer);
	m_WorldTextBlock.Render(renderer);
	m_TimeTextBlock.Render(renderer);
	m_WorldNumberTextBlock.Render(renderer);
	m_CoinNumberTextBlock.Render(renderer);

	std::stringstream timerStream;
	timerStream << std::setw(3) << std::setfill('0') << (uint32_t)ceilf(m_Timer);
	const std::string timerText = timerStream.str();
	fontHandler.RenderText(renderer, m_pTextFont, timerText, { m_pApplication->GetWindowCenter().x + fontHandler.GetTextSize(m_pTextFont, timerText).x * 3.8f, 25.0f }, { 250, 250, 250, 255 });
}

void CGameState::RenderDebug(void)
{
	m_pTilemap->RenderDebug();
	m_pPlayer->RenderDebug();
	m_pGoombas->RenderDebug();
	m_pPipeUpperLeft->RenderDebug();
	m_pPipeUpperRight->RenderDebug();
	m_pPipeLowerLeft->RenderDebug();
	m_pPipeLowerRight->RenderDebug();
}

// This function is called whenever the player is playing its dying animation
void CGameState::OnPlayerDying(void)
{
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();

	if (!m_isDeadSound) 
	{
		audioHandler.StopMusic();
		audioHandler.PlayMusic(e_pDeathSound, -1);
		audioHandler.SetMusicVolume(15);
	}

	m_DeathFadeDelay = m_DeathFadeDelayDefault;
	m_DeathFadeout = true;

	m_State = Estate::ROUND_ENDED;

	e_EndOfRoundPlayerKilled = true;
}