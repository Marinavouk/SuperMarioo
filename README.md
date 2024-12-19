# Super Mario - My Mini Version

---------------------------
# My Examination Project for Forsbergs Skola December 2024
# Game Programming 2023
---------------------------

![image](https://github.com/user-attachments/assets/7f2751e1-fabf-4c66-99bc-b41e9f2f4097)

I decided to develop my skills in C++ by using the SDL library and creating a game from scratch for my examination project. It was a challenging but rewarding 7 weeks.

During this project, I:

Worked with the SDL library to deepen my knowledge of C++.
Created functions and implemented mechanics for the game.
Completed a simple 2D game with a scoring system.
Learned key concepts such as tilemaps, animations, colliders, and movement, while focusing on organizing code to be readable and maintainable.
Enjoyed the process of making mistakes, debugging, and researching solutions for hours.

-----------------------------
Some exampel of GameState.CPP 
-----------------------------

```
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

	audioHandler.PlayMusic(m_pMusic, -1);

	m_GoombaCount = 0;

	e_GoombaCount = 0;
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
		
		m_pPlayer->HandleInput(deltaTime);
		m_pPlayer->Update(deltaTime);
		m_pPlayer->HandleTilemapCollision(m_pTilemap->GetColliders(), deltaTime);
		m_pPlayer->HandlePipeCollision(m_Pipes, deltaTime);
		m_pPlayer->HandleEnemyCollision(m_Enemies, deltaTime);

		m_pGoomba1->Update(deltaTime);
		m_pGoomba1->HandleTilemapCollision(m_pTilemap->GetColliders(), deltaTime);
		m_pGoomba1->HandlePipeCollision(m_Pipes, deltaTime);

		m_Timer -= deltaTime;

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

			e_GoombaCount = m_GoombaCount;
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
```
------------------------------
Here is an example of the Tilemap.cpp file. Tiles are defined in a 2D array (map), where each number represents a specific tile type. At the same time, a collider is created for each non-zero tile.
------------------------------

```
#include "Pch.h"
#include "Tilemap.h"

#include "Application.h"

bool CTilemap::Create(CApplication* application)
{
	m_pApplication = application;

	m_pTexture = m_pApplication->GetTextureHandler().CreateTexture("tiles.png");

	const int32_t numRows = 13;
	const int32_t numColumns = 16;

	const int32_t map[numRows][numColumns] =
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	};

	for (int32_t i = 0; i < numRows; ++i)
	{
		std::vector<STile> Tiles = {};

		for (int32_t j = 0; j < numColumns; ++j)
		{
			const int32_t TileType = map[i][j];

			if (TileType != 0)
			{
				STile tile = { .m_Position = { (j * m_TileSize.x), (i * m_TileSize.y)}, .m_TexCoord = {(TileType - 1) * (int)m_TileSize.x, 0} };
				Tiles.push_back(tile);

				m_TileColliders.push_back({ tile.m_Position.x, tile.m_Position.y, m_TileSize.x, m_TileSize.y });
			}
		}

		if (!Tiles.empty())
			m_Tiles.push_back(Tiles);
	}

	return true;
}

void CTilemap::Destroy(void)
{
	m_Tiles.clear();

	m_pApplication->GetTextureHandler().DestroyTexture(m_pTexture->GetName());
	m_pTexture = nullptr;

	m_pApplication = nullptr;
}

void CTilemap::Render(void)
{
	/*
	for (int i = 0; i < m_Tiles.size(); ++i)
	{
		for (int j = 0; j < m_Tiles[i].size(); ++j)
		{

		}
	}
	*/

	// Range-based for loop
	// This works the same as the outer-most for-loop commented out above
	for (const TileVector& row : m_Tiles)
	{
		for (const STile& tile : row)
		{
			const SDL_FRect	rectangle = { tile.m_Position.x, tile.m_Position.y, m_TileSize.x, m_TileSize.y };
			const SDL_Rect	clipRectangle = { tile.m_TexCoord.x, tile.m_TexCoord.y, (int)m_TileSize.x, (int)m_TileSize.y };

			m_pTexture->SetTextureCoords(clipRectangle);
			m_pTexture->Render(tile.m_Position, &rectangle);
		}
	}
```

----------------------------------
Last but not least the Player.cpp. I Struggled with colliders, which were insanely difficult to build and understand.
----------------------------------

```
#include "Pch.h"
#include "Player.h"

#include "Application.h"
#include "Utilities/CollisionUtilities.h"

bool CPlayer::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	SDL_FPoint frameSize = { 28.0f, 28.0f };

	m_pAnimatorIdle = new CAnimator;
	m_pAnimatorWalking = new CAnimator;
	m_pAnimatorRunning = new CAnimator;
	m_pAnimatorJumping = new CAnimator;
	m_pAnimatorDying = new CAnimator;
	m_pAnimatorIdle->Set(m_pTexture, 1, 0, 0, 0, frameSize, 8.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorWalking->Set(m_pTexture, 3, 1, 3, 0, frameSize, 8.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorRunning->Set(m_pTexture, 3, 1, 3, 0, frameSize, 16.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorJumping->Set(m_pTexture, 1, 5, 5, 0, frameSize, 8.0f, false, CAnimator::EDirection::FORWARD);
	m_pAnimatorDying->Set(m_pTexture, 1, 7, 7, 0, frameSize, 8.0f, false, CAnimator::EDirection::FORWARD);

	ActivateAnimator(m_pAnimatorIdle);

	m_pTexture->SetSize({ frameSize.x * m_Scale, frameSize.y * m_Scale });
	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());

	m_Rectangle = { position.x, position.y, frameSize.x * m_Scale, frameSize.y * m_Scale };

	m_HorizontalCollider = { m_Rectangle.x + m_HorizontalColliderOffset.x, m_Rectangle.y + m_HorizontalColliderOffset.y, 15.0f * m_Scale, 9.0f * m_Scale };
	m_VerticalCollider = { m_Rectangle.x + m_VerticalColliderOffset.x, m_Rectangle.y + m_VerticalColliderOffset.y, 10.0f * m_Scale, 24.0f * m_Scale };

	m_Collider = { m_VerticalCollider.x, m_VerticalCollider.y, m_VerticalCollider.w, m_VerticalCollider.h };

	return true;
}

void CPlayer::Destroy(void)
{
	m_pEnemyStompCallback = nullptr;
	m_pExitingPipeCallback = nullptr;
	m_pEnteringPipeCallback = nullptr;
	m_pDyingCallback = nullptr;
	m_pJumpingCallback = nullptr;

#define DELETE_ANIMATOR(Animator) delete Animator; Animator = nullptr;

	DELETE_ANIMATOR(m_pAnimatorDying);
	DELETE_ANIMATOR(m_pAnimatorJumping);
	DELETE_ANIMATOR(m_pAnimatorRunning);
	DELETE_ANIMATOR(m_pAnimatorWalking);
	DELETE_ANIMATOR(m_pAnimatorIdle);

#undef DELETE_ANIMATOR

	m_pCurrentAnimator = nullptr;

	CGameObject::Destroy();
}

void CPlayer::Kill(void)
{
	CGameObject::Kill();

	ActivateAnimator(m_pAnimatorDying);

	m_Velocity = { 0.0f, -m_DyingJumpStrength }; 

	m_State = EState::DEAD;

	m_IsDyingJumping = true;

	if (m_pDyingCallback)
		m_pDyingCallback();
}

void CPlayer::RenderDebug(void)
{
	CGameObject::RenderDebug();

	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 255, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_HorizontalCollider);

	SDL_SetRenderDrawColor(renderer, 0, 200, 200, 255);
	SDL_RenderDrawRectF(renderer, &m_VerticalCollider);
}

void CPlayer::Update(const float deltaTime)
{
	m_Velocity.y = std::min(m_Velocity.y + m_Gravity * deltaTime, m_MaxFallVelocity);

	m_Rectangle.x += m_Velocity.x * deltaTime;
	m_Rectangle.y += m_Velocity.y * deltaTime;

	SyncColliders();

	if (m_State == EState::NORMAL)
	{
		const SDL_FPoint windowSize = m_pApplication->GetWindowSize();

		// Wrap-around mechanic
		// When Mario leaves the left side of the window, move Mario to the right side of the window, and vice versa
		if ((m_Rectangle.x + (m_Rectangle.w * 0.5f)) < 0.0f) m_Rectangle.x = windowSize.x - (m_Rectangle.w * 0.5f);
		else if ((m_Rectangle.x + (m_Rectangle.w * 0.5f)) > windowSize.x) m_Rectangle.x = -(m_Rectangle.w * 0.5f);

		if (m_Rectangle.y > (windowSize.y - m_Rectangle.h))
		{
			m_Rectangle.y = windowSize.y - m_Rectangle.h;

			m_Velocity.y = 0.0f;

			if (m_IsJumping)
			{
				if ((m_pCurrentAnimator != m_pAnimatorIdle) && (m_HorizontalDirection == EMovementState::IDLE) && (m_VerticalDirection == EMovementState::IDLE))
					ActivateAnimator(m_pAnimatorIdle);

				else
				{
					if (m_HorizontalDirection != EMovementState::IDLE)
					{
						if (m_IsRunning)
							ActivateAnimator(m_pAnimatorRunning);

						else
							ActivateAnimator(m_pAnimatorWalking);
					}
				}
			}

			m_IsJumping = false;
		}
	}

	else if (m_State == EState::DEAD)
	{
		if (m_IsDyingJumping)
		{
			m_Velocity.y = std::min(m_Velocity.y + m_DyingGravity * deltaTime, m_MaxFallVelocity);

			if (m_Velocity.y >= 0.0f)
				m_IsDyingJumping = false;
		}

		else
			m_Velocity.y = std::min(m_Velocity.y + m_Gravity * deltaTime, m_MaxFallVelocity);
	}

	else if (m_State == EState::ENTERING_PIPE)
	{
		bool inPipe = false;

		if (m_HorizontalDirection == EMovementState::MOVING_LEFT)
		{
			if (m_Rectangle.x < 0.0f)
				inPipe = true;
		}

		else if (m_HorizontalDirection == EMovementState::MOVING_RIGHT)
		{
			if ((m_Rectangle.x + m_Rectangle.w) > m_pApplication->GetWindowSize().x)
				inPipe = true;
		}

		if (inPipe)
		{
			const bool exitPipeFacingRight = (m_ExitPipeDirection == EMovementState::MOVING_RIGHT);

			m_pTexture->SetFlipMethod((exitPipeFacingRight ? SDL_RendererFlip::SDL_FLIP_NONE : SDL_RendererFlip::SDL_FLIP_HORIZONTAL));

			m_FlipMethod = m_pTexture->GetFlipMethod();

			m_PipeTimeout = m_PipeTimeoutDefault;

			m_HorizontalDirection = (exitPipeFacingRight ? EMovementState::MOVING_RIGHT : EMovementState::MOVING_LEFT);

			m_Velocity.x = 0.0f;

			m_State = EState::IN_PIPE;

			m_Rectangle.x = m_ExitPipePosition.x;
			m_Rectangle.y = m_ExitPipePosition.y;

			SyncColliders();

			ActivateAnimator(m_pAnimatorIdle);
		}
	}

	else if (m_State == EState::IN_PIPE)
	{
		m_PipeTimeout -= deltaTime;

		if (m_PipeTimeout <= 0.0f)
		{
			m_Velocity.x = ((m_HorizontalDirection == EMovementState::MOVING_LEFT) ? -m_PipeTraversingVelocity : m_PipeTraversingVelocity);

			m_PipeTimeout = 0.0f;

			m_State = EState::EXITING_PIPE;

			ActivateAnimator(m_pAnimatorWalking);

			if (m_pExitingPipeCallback)
				m_pExitingPipeCallback();
		}
	}

	SyncColliders();

	if (m_pCurrentAnimator)
		m_pCurrentAnimator->Update(deltaTime);
}

void CPlayer::HandleInput(const float deltaTime)
{
	if (m_State != EState::NORMAL)
		return;

	CInputHandler& inputHandler = m_pApplication->GetInputHandler();

	// Pressed keys

	if (inputHandler.KeyPressed(SDL_SCANCODE_SPACE) && m_OnGround && !m_IsJumping)
	{
		m_Velocity.y = -m_JumpStrength;

		m_pCurrentAnimator = m_pAnimatorJumping;
		m_pCurrentAnimator->Reset();

		m_OnGround = false;
		m_IsJumping = true;

		if (m_pJumpingCallback)
			m_pJumpingCallback();
	}

	// Held keys

	m_IsRunning = inputHandler.KeyHeld(SDL_SCANCODE_S);

	if (inputHandler.KeyHeld(SDL_SCANCODE_LEFT) && !inputHandler.KeyHeld(SDL_SCANCODE_RIGHT))
	{
		const float acceleration = (m_IsRunning ? m_AccelerationSpeedRunning : m_AccelerationSpeedWalking) * deltaTime;
		const float maxVelocity = (m_IsRunning ? -m_MaxRunningVelocity : -m_MaxWalkingVelocity);

		m_Velocity.x = std::max(m_Velocity.x - acceleration, maxVelocity);

		m_pTexture->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

		m_FlipMethod = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;

		m_HorizontalDirection = EMovementState::MOVING_LEFT;

		if (!m_IsJumping)
		{
			if (m_IsRunning)
				ActivateAnimator(m_pAnimatorRunning);

			else
				ActivateAnimator(m_pAnimatorWalking);
		}
	}

	else if (inputHandler.KeyHeld(SDL_SCANCODE_RIGHT) && !inputHandler.KeyHeld(SDL_SCANCODE_LEFT))
	{
		const float acceleration = (m_IsRunning ? m_AccelerationSpeedRunning : m_AccelerationSpeedWalking) * deltaTime;
		const float maxVelocity = (m_IsRunning ? m_MaxRunningVelocity : m_MaxWalkingVelocity);

		m_Velocity.x = std::min(m_Velocity.x + acceleration, maxVelocity);

		m_pTexture->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_NONE);

		m_FlipMethod = SDL_RendererFlip::SDL_FLIP_NONE;

		m_HorizontalDirection = EMovementState::MOVING_RIGHT;

		if (!m_IsJumping)
		{
			if (m_IsRunning)
				ActivateAnimator(m_pAnimatorRunning);

			else
				ActivateAnimator(m_pAnimatorWalking);
		}
	}

	else
	{
		const float speed = (m_IsRunning ? m_DeaccelerationSpeedRunning : m_DeaccelerationSpeedWalking) * deltaTime;

		if (m_Velocity.x < 0.0f) m_Velocity.x = std::min(0.0f, m_Velocity.x + speed);
		else if (m_Velocity.x > 0.0f) m_Velocity.x = std::max(0.0f, m_Velocity.x - speed);
	}

	// Released keys

	if (inputHandler.KeyReleased(SDL_SCANCODE_LEFT) && (m_HorizontalDirection == EMovementState::MOVING_LEFT))
	{
		m_HorizontalDirection = EMovementState::IDLE;

		if ((m_VerticalDirection == EMovementState::IDLE) && !m_IsJumping)
			ActivateAnimator(m_pAnimatorIdle);
	}

	else if (inputHandler.KeyReleased(SDL_SCANCODE_RIGHT) && (m_HorizontalDirection == EMovementState::MOVING_RIGHT))
	{
		m_HorizontalDirection = EMovementState::IDLE;

		if ((m_VerticalDirection == EMovementState::IDLE) && !m_IsJumping)
			ActivateAnimator(m_pAnimatorIdle);
	}

	if ((m_HorizontalDirection == EMovementState::IDLE) && (m_VerticalDirection == EMovementState::IDLE) && !m_IsJumping)
		ActivateAnimator(m_pAnimatorIdle);
}

void CPlayer::HandleTilemapCollision(const CTilemap::TileColliders& tilemapColliders, const float deltaTime)
{
	const SDL_FPoint moveAmount = {m_Velocity.x * deltaTime, m_Velocity.y * deltaTime};

	for (const SDL_FRect& collider : tilemapColliders)
	{
		if (ResolveXCollision(collider, moveAmount))
			break;

		if (ResolveTileYCollision(collider, moveAmount))
			break;
	}
}

void CPlayer::HandlePipeCollision(const GameObjectList& pipes, const float deltaTime)
{
	if ((m_State == EState::DEAD) || (m_State == EState::ENTERING_PIPE))
		return;

	const SDL_FPoint moveAmount = { m_Velocity.x * deltaTime, m_Velocity.y * deltaTime };

	if (m_State == EState::NORMAL)
	{
		for (CGameObject* pipe : pipes)
		{
			if (ResolveXCollision(pipe->GetCollider(), moveAmount))
			{
				CGameObject* exitPipe = pipes[m_pApplication->GetRandomNumberGenerator()->RandomUint(0, (uint32_t)pipes.size() - 1)];
				const SDL_FPoint exitPipePosition = exitPipe->GetRectanglePosition();
				const SDL_FPoint exitPipeSize = exitPipe->GetRectangleSize();
				const bool enterRightFacingPipe = (pipe->GetFlipMethod() == SDL_RendererFlip::SDL_FLIP_NONE);
				const bool exitRightFacingPipe = (exitPipe->GetFlipMethod() == SDL_RendererFlip::SDL_FLIP_NONE);

				m_pTexture->SetFlipMethod((enterRightFacingPipe ? SDL_RendererFlip::SDL_FLIP_HORIZONTAL : SDL_RendererFlip::SDL_FLIP_NONE));

				m_HorizontalDirection = (enterRightFacingPipe ? EMovementState::MOVING_LEFT : EMovementState::MOVING_RIGHT);

				m_ExitPipeDirection = (exitRightFacingPipe ? EMovementState::MOVING_RIGHT : EMovementState::MOVING_LEFT);

				m_ExitPipePosition.x = (exitRightFacingPipe ? (exitPipePosition.x + exitPipeSize.x) - m_Rectangle.w : exitPipePosition.x);
				m_ExitPipePosition.y = (exitPipePosition.y + exitPipeSize.y) - m_Rectangle.h;

				m_Velocity.x = (enterRightFacingPipe ? -m_PipeTraversingVelocity : m_PipeTraversingVelocity);

				m_State = EState::ENTERING_PIPE;

				SyncColliders();

				ActivateAnimator(m_pAnimatorWalking);

				if (m_pEnteringPipeCallback)
					m_pEnteringPipeCallback();
			}
		}
	}

	else if (m_State == EState::EXITING_PIPE)
	{
		bool pipeCollision = false;

		for (CGameObject* pipe : pipes)
		{
			SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

			if (QuadVsQuad(m_Rectangle, pipe->GetCollider(), &intersection))
			{
				pipeCollision = true;

				break;
			}
		}

		if (!pipeCollision)
		{
			m_Velocity.x = 0.0f;

			m_State = EState::NORMAL;

			ActivateAnimator(m_pAnimatorIdle);
		}
	}
}

void CPlayer::HandleEnemyCollision(const GameObjectList& enemies, const float deltaTime)
{
	if (m_IsDead)
		return;

	const SDL_FPoint moveAmount = { m_Velocity.x * deltaTime, m_Velocity.y * deltaTime };

	for (CGameObject* enemy : enemies)
	{
		if (enemy->GetIsDead() || !enemy->GetIsActive())
			continue;

		if (ResolveXCollision(enemy->GetCollider(), moveAmount))
		{
			Kill();

			break;
		}

		if (ResolveEnemyYCollision(enemy->GetCollider(), moveAmount))
		{
			enemy->Kill();

			if (m_pEnemyStompCallback)
				m_pEnemyStompCallback();

			break;
		}
	}
}

bool CPlayer::ResolveTileYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollided = false;

	m_OnGround = false;

	// The player is moving up (jumping)
	if (moveAmount.y < 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_VerticalCollider, collider, &intersection))
		{
			m_Rectangle.y += intersection.h;

			m_Velocity.y = 0.0f;

			hasCollided = true;
		}
	}

	// The player is moving down
	else if (moveAmount.y > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_VerticalCollider, collider, &intersection))
		{
			m_Rectangle.y -= intersection.h;

			m_Velocity.y = 0.0f;

			m_OnGround = true;
			m_IsJumping = false;

			hasCollided = true;
		}
	}

	if (hasCollided)
		SyncColliders();

	return hasCollided;
}

bool CPlayer::ResolveEnemyYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollided = false;

	// The player is moving down
	if (moveAmount.y > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_VerticalCollider, collider, &intersection))
		{
			m_Rectangle.y -= intersection.h;

			m_Velocity.y = -m_EnemyStompJumpStrength;

			hasCollided = true;

			if (m_pEnemyStompCallback)
				m_pEnemyStompCallback();
		}
	}

	if (hasCollided)
		SyncColliders();

	return hasCollided;
}

bool CPlayer::ResolveXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollided = false;

	// The player is moving to the left
	if (moveAmount.x < 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x += intersection.w;

			m_Velocity.x = 0.0f;

			hasCollided = true;
		}
	}

	// The player is moving to the right
	else if (moveAmount.x > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x -= intersection.w;

			m_Velocity.x = 0.0f;

			hasCollided = true;
		}
	}

	// Player is standing still
	else
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x += intersection.w;

			m_Velocity.x = 0.0f;

			hasCollided = true;
		}

		if (moveAmount.y != 0.0f)
		{
			SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

			if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
			{
				m_Rectangle.x += ((m_HorizontalCollider.x < collider.x) ? -intersection.w : intersection.w);

				SyncColliders();

				m_Velocity.x = 0.0f;

				hasCollided = true;
			}
		}
	}

	if (hasCollided)
		SyncColliders();

	return hasCollided;
}

void CPlayer::SyncColliders(void)
{
	m_HorizontalCollider.x = m_Rectangle.x + m_HorizontalColliderOffset.x;
	m_HorizontalCollider.y = m_Rectangle.y + m_HorizontalColliderOffset.y;
	m_VerticalCollider.x = m_Rectangle.x + m_VerticalColliderOffset.x;
	m_VerticalCollider.y = m_Rectangle.y + m_VerticalColliderOffset.y;

	m_Collider = { m_VerticalCollider.x, m_VerticalCollider.y , m_VerticalCollider.w, m_VerticalCollider.h };
}

void CPlayer::ActivateAnimator(CAnimator* animator)
{
	if (m_pCurrentAnimator != animator)
	{
		m_pCurrentAnimator = animator;
		m_pCurrentAnimator->Reset();
	}
}
```
-------------------------

