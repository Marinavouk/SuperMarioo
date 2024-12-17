#include "Pch.h"
#include "GameState.h"

#include "Application.h"
#include "GameObjects/Goomba.h"
#include "GameObjects/Pipe.h"
#include "GameObjects/Player.h"
#include "Globals.h"
#include "Utilities/CollisionUtilities.h"

#include <iomanip>
#include <sstream>

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
	if (!m_pPlayer->Create("mario.png", { 0.0f, 0.0f }, 1))
		return false;
	m_pPlayer->SetPosition({ 80.0f, (windowSize.y - m_pPlayer->GetRectangleSize().y) - tileSize.y });
	CPlayer* player = (CPlayer*)m_pPlayer;
	player->SetJumpingCallback(std::bind(&CGameState::OnPlayerJumping, this));
	player->SetDyingCallback(std::bind(&CGameState::OnPlayerDying, this));
	player->SetEnteringPipeCallback(std::bind(&CGameState::OnPlayerEnteringPipe, this));
	player->SetExitingPipeCallback(std::bind(&CGameState::OnPlayerExitingPipe, this));
	player->SetEnemyStompCallback(std::bind(&CGameState::OnPlayerEnemyStomp, this));

	m_pGoomba1 = new CGoomba(m_pApplication);
	if (!m_pGoomba1->Create("goomba.png", { 0.0f, 0.0f }, 1))
		return false;
	m_pGoomba1->SetPosition({ 300.0f, (windowSize.y - m_pGoomba1->GetRectangleSize().y) - tileSize.y });
	CGoomba* goomba1 = (CGoomba*)m_pGoomba1;
	goomba1->SetDyingCallback(std::bind(&CGameState::OnEnemyDead, this, std::placeholders::_1));

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

	m_Pipes.push_back(m_pPipeUpperLeft);
	m_Pipes.push_back(m_pPipeUpperRight);
	m_Pipes.push_back(m_pPipeLowerLeft);
	m_Pipes.push_back(m_pPipeLowerRight);

	m_Enemies.push_back(m_pGoomba1);

	m_pCoin = textureHandler.CreateTexture("coin.png");
	m_pCoin->SetSize({ 16.f, 16.f });

	m_pTextFont = m_pApplication->GetFontHandler().CreateFont("Assets/Fonts/VCR_OSD_MONO.ttf", 18);
	if (!m_pTextFont)
		return false;

	m_pMusic = audioHandler.CreateMusic("Assets/Audio/underground.mp3");
	if (!m_pMusic)
		return false;

	m_pHurryMusic = audioHandler.CreateMusic("Assets/Audio/underground_hurry.mp3");
	if (!m_pHurryMusic)
		return false;

	m_pJumpSound = audioHandler.CreateSound("Assets/Audio/jump.wav");
	if (!m_pJumpSound)
		return false;

	m_pPipeSound = audioHandler.CreateSound("Assets/Audio/pipe.wav");
	if (!m_pPipeSound)
		return false;

	m_pDeathSound = audioHandler.CreateSound("Assets/Audio/death.wav");
	if (!m_pDeathSound)
		return false;

	m_pGoombaSound = audioHandler.CreateSound("Assets/Audio/goomba.wav");
	if (!m_pGoombaSound)
		return false;

	const SDL_Color titleTextColor = { 255,	255, 255, 255 }; // White

	// Buttons can be used as text blocks too, without mouse interaction
	if (!m_MarioTextBlock.Create(m_pApplication, m_pTextFont, "MARIO", titleTextColor))
		return false;
	m_MarioTextBlock.SetPosition({ 30.0f, 10.0f });
	m_MarioTextBlock.SetBackgroundColor({ 0, 0, 0, 0 }); // Only the text in the text block should be visible, so the background is set to be invisible (alpha = 0)

	if (!m_WorldTextBlock.Create(m_pApplication, m_pTextFont, "WORLD", titleTextColor))
		return false;
	m_WorldTextBlock.SetPosition({ windowCenter.x + 10.0f, 10.0f });
	m_WorldTextBlock.SetBackgroundColor({ 0, 0, 0, 0 });

	if (!m_TimeTextBlock.Create(m_pApplication, m_pTextFont, "TIME", titleTextColor))
		return false;
	m_TimeTextBlock.SetPosition({ windowCenter.x + 150.0f, 10.0f });
	m_TimeTextBlock.SetBackgroundColor({ 0, 0, 0, 0 });

	if (!m_WorldNumberTextBlock.Create(m_pApplication, m_pTextFont, "0-88", titleTextColor))
		return false;
	m_WorldNumberTextBlock.SetPosition({ windowCenter.x + 70.0f, 20.0f });
	m_WorldNumberTextBlock.SetBackgroundColor({ 0, 0, 0, 0 });
	
	if (!m_CoinNumberTextBlock.Create(m_pApplication, m_pTextFont, "x00", titleTextColor))
		return false;
	m_CoinNumberTextBlock.SetPosition({ 185.0f, 18.0f });
	m_CoinNumberTextBlock.SetBackgroundColor({ 0, 0, 0, 0 });

	m_Timer = m_TimerDefault;

	m_DeathFadeout = false;

	m_State = Estate::IDLE;

//	audioHandler.PlayMusic(m_pMusic, -1);

	e_EndOfRoundPlayerKilled = false;

	return true;
}

void CGameState::OnExit(void)
{
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();

#define DESTROY_SOUND(Sound) audioHandler.DestroySound(Sound); Sound = nullptr;

	DESTROY_SOUND(m_pGoombaSound);
	DESTROY_SOUND(m_pDeathSound);
	DESTROY_SOUND(m_pPipeSound);
	DESTROY_SOUND(m_pJumpSound);

#undef DESTROY_SOUND

#define DESTROY_MUSIC(Music) audioHandler.DestroyMusic(Music); Music = nullptr;

	DESTROY_MUSIC(m_pHurryMusic);
	DESTROY_MUSIC(m_pMusic);

#undef DESTROY_MUSIC

	m_CoinNumberTextBlock.Destroy(m_pApplication);
	m_WorldNumberTextBlock.Destroy(m_pApplication);
	m_TimeTextBlock.Destroy(m_pApplication);
	m_WorldTextBlock.Destroy(m_pApplication);
	m_MarioTextBlock.Destroy(m_pApplication);

	m_pApplication->GetFontHandler().DestroyFont(m_pTextFont);
	m_pTextFont = nullptr;

	textureHandler.DestroyTexture(m_pCoin->GetName());
	m_pCoin = nullptr;

#define DESTROY_GAME_OBJECT(GameObject) GameObject->Destroy(); delete GameObject; GameObject = nullptr;

	DESTROY_GAME_OBJECT(m_pPipeLowerRight);
	DESTROY_GAME_OBJECT(m_pPipeLowerLeft);
	DESTROY_GAME_OBJECT(m_pPipeUpperRight);
	DESTROY_GAME_OBJECT(m_pPipeUpperLeft);
	DESTROY_GAME_OBJECT(m_pGoomba1);
	DESTROY_GAME_OBJECT(m_pPlayer);
	DESTROY_GAME_OBJECT(m_pTilemap);

#undef DESTROY_GAME_OBJECT

	m_Enemies.clear();
	m_Pipes.clear();
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
		m_pPlayer->HandlePipeCollision(m_Pipes, deltaTime);
		m_pPlayer->HandleEnemyCollision(m_Enemies, deltaTime);

		m_pGoomba1->Update(deltaTime);
		m_pGoomba1->HandleTilemapCollision(m_pTilemap->GetColliders(), deltaTime);
		m_pGoomba1->HandlePipeCollision(m_Pipes, deltaTime);

	//	m_Timer -= deltaTime;

		if (m_Timer <= 60.0f && !m_HurryMusicStarted)
		{
			audioHandler.StopMusic();
			audioHandler.PlayMusic(m_pHurryMusic, -1);

			m_HurryMusicStarted = true;
		}

		else if (m_Timer <= 0.0f)
		{		
			m_Timer = 0.0f;

			m_State = Estate::ROUND_ENDED;

			e_EndOfRoundPlayerKilled = false;

			m_pApplication->SetState(CApplication::EState::END_OF_ROUND);
		}
	}

	else if (m_State == Estate::ROUND_ENDED)
	{
		m_pPlayer->Update(deltaTime);

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

	for (CGameObject* goomba : m_Enemies)
	{
		if (!goomba->GetIsDead())
			goomba->Render();
	}

	m_pPlayer->Render();

	for (CGameObject* pipe : m_Pipes)
	{
		pipe->Render();
	}

	m_MarioTextBlock.Render(renderer);
	m_WorldTextBlock.Render(renderer);
	m_TimeTextBlock.Render(renderer);
	m_WorldNumberTextBlock.Render(renderer);
	m_CoinNumberTextBlock.Render(renderer);

	m_pCoin->Render({ 155.f, 10.0f });

	std::stringstream timerStream;
	timerStream << std::setw(3) << std::setfill('0') << (uint32_t)ceilf(m_Timer);
	const std::string timerText = timerStream.str();
	fontHandler.RenderText(renderer, m_pTextFont, timerText, { m_pApplication->GetWindowCenter().x + fontHandler.GetTextSize(m_pTextFont, timerText).x * 5.5f, 12.0f }, { 250, 250, 250, 255 });
}

void CGameState::RenderDebug(void)
{
	m_pTilemap->RenderDebug();
	m_pPlayer->RenderDebug();
	m_pGoomba1->RenderDebug();
	m_pPipeUpperLeft->RenderDebug();
	m_pPipeUpperRight->RenderDebug();
	m_pPipeLowerLeft->RenderDebug();
	m_pPipeLowerRight->RenderDebug();
}

// This function is called once, when Mario is jumping
void CGameState::OnPlayerJumping(void)
{
	m_pApplication->GetAudioHandler().PlaySound(m_pJumpSound);
}

// This function is called once, when Mario is dying
void CGameState::OnPlayerDying(void)
{
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();

	audioHandler.StopMusic();
	audioHandler.PlaySound(m_pDeathSound);

	m_DeathFadeDelay = m_DeathFadeDelayDefault;
	m_DeathFadeout = true;

	m_State = Estate::ROUND_ENDED;

	e_EndOfRoundPlayerKilled = true;

	audioHandler.StopMusic();
	audioHandler.PlaySound(m_pDeathSound);
}

// This function is called once, when Mario enters a pipe
void CGameState::OnPlayerEnteringPipe(void)
{
	m_pApplication->GetAudioHandler().PlaySound(m_pPipeSound);
}

// This function is called once, when Mario exits a pipe
void CGameState::OnPlayerExitingPipe(void)
{
	m_pApplication->GetAudioHandler().PlaySound(m_pPipeSound);
}

void CGameState::OnPlayerEnemyStomp(void)
{
	m_pApplication->GetAudioHandler().PlaySound(m_pGoombaSound);
}

void CGameState::OnEnemyDead(CGameObject* enemy)
{
	// TODO: re-spawn the goomba in a random pipe

	printf("Goomba has died");
}