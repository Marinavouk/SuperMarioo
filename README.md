# Super Mario VS Goombas

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
	player->SetEnemyStompCallback(std::bind(&CGameState::OnGoombaStomped, this));

	for (uint32_t i = 0; i < 5; ++i)
	{
		CGameObject* gameObject = new CGoomba(m_pApplication);
		if (!gameObject->Create("goomba.png", { -1000.0f, -1000.0f }, 1))
			return false;

		CGoomba* goomba = (CGoomba*)gameObject;
		goomba->SetDyingCallback(std::bind(&CGameState::OnGoombaDead, this, std::placeholders::_1));
		goomba->SetIndex(i);

		m_GoombaPool.push_back(gameObject);
	}

	for (uint32_t i = 0; i < 5; ++i)
	{
		SpawnGoomba();
	}

	m_pGoombaGUI = textureHandler.CreateTexture("goomba2.png");
	m_pGoombaGUI->SetSize({ 16.f, 16.f });

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


	for (CGameObject* goomba : m_GoombaPool)
	{
		goomba->Destroy();
		delete goomba;
	}

	m_GoombaPool.clear();
	m_ActiveGoombas.clear();
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
		m_pPlayer->HandleEnemyCollision(m_ActiveGoombas, deltaTime);

		for (CGameObject* goomba : m_ActiveGoombas)
		{
			if (!goomba->GetIsDead()) 
			{
				goomba->Update(deltaTime);
				goomba->HandleTilemapCollision(m_pTilemap->GetColliders(), deltaTime);
				goomba->HandlePipeCollision(m_Pipes, deltaTime);
			}

		}
```
------------------------------
Here is an example of the Tilemap.cpp file. Tiles are defined in a 2D array (map), where each number represents a specific tile type. At the same time, a collider is created for each  tile.
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

void CTilemap::Render(void)
{
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
Last but not least the Player.cpp. I struggled with colliders, which were insanely difficult to build and understand.
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
			m_Rectangle.y = (windowSize.y - m_Rectangle.h);

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

			m_OnGround = true;
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

