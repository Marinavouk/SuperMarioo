#include "Pch.h"
#include "Player.h"

#include "Application.h"
#include "Utilities/CollisionUtilities.h"
bool CPlayer::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{

	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	const SDL_FPoint frameSize = { 64.0f, 128.0f };

	m_pAnimatorIdle = new CAnimator;
	m_pAnimatorWalking = new CAnimator;
	m_pAnimatorRunning = new CAnimator;
	m_pAnimatorJumping = new CAnimator;
	m_pAnimatorDying = new CAnimator;




    return false;
}

void CPlayer::Destroy(void)
{
}

void CPlayer::Kill(void)
{
}

void CPlayer::Render(void)
{
}

void CPlayer::RenderDebug(void)
{
}

void CPlayer::Update(const float deltaTime)
{
}

void CPlayer::HandleInput(const float deltaTime)
{
}

void CPlayer::HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime)
{
}

void CPlayer::HandleEnemyCollision(const GameObjectList& enemies, const float deltaTime)
{
}
