#include "Pch.h"
#include "Player.h"

bool CPlayer::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
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
