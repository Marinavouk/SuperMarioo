#include "Pch.h"
#include "Goombas.h"

bool CGoombas::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	const SDL_FPoint frameSize = { 28.0f, 28.0f };

	m_pAnimatorWalking = new CAnimator;
	m_pAnimatorDying = new CAnimator;
	m_pAnimatorWalking->Set(m_pTexture, 10, 0, 9, 3, frameSize, 7.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorDying->Set(m_pTexture, 4, 0, 3, 4, frameSize, 7.0f, false, CAnimator::EDirection::FORWARD);

	m_pAnimatorDying->SetAnimationEndCallback(std::bind(&CGoombas::OnDyingAnimationEnd, this));


	return false;
}

void CGoombas::Destroy(void)
{
}

void CGoombas::Kill(void)
{
}

void CGoombas::Render(void)
{
}

void CGoombas::Update(const float deltaTime)
{
}

void CGoombas::HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime)
{
}


