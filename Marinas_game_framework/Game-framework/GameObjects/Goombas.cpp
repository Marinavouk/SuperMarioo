#include "Pch.h"
#include "Goombas.h"

#include "Application.h"
#include "Utilities/CollisionUtilities.h"

bool CGoombas::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	const SDL_FPoint frameSize = { 30.0f, 30.0f };

	m_pAnimatorWalking = new CAnimator;
	m_pAnimatorDying = new CAnimator;
	m_pAnimatorWalking->Set(m_pTexture, 2, 0, 1, 0, frameSize, 7.0f, true, CAnimator::EDirection::FORWARD);
	m_pAnimatorDying->Set(m_pTexture, 1, 2, 2, 0, frameSize, 7.0f, false, CAnimator::EDirection::FORWARD);

	ActivateAnimator(m_pAnimatorWalking);

	m_pTexture->SetSize({ frameSize.x * m_Scale, frameSize.y * m_Scale });
	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());

	m_Rectangle = { position.x, position.y, frameSize.x * m_Scale, frameSize.y * m_Scale };

	m_HorizontalCollider = { m_Rectangle.x + m_HorizontalColliderOffset.x,	m_Rectangle.y + m_HorizontalColliderOffset.y,	15.0f * m_Scale, 9.0f * m_Scale };
	m_VerticalCollider = { m_Rectangle.x + m_VerticalColliderOffset.x,		m_Rectangle.y + m_VerticalColliderOffset.y,		10.0f * m_Scale, 24.0f * m_Scale };

	m_Collider = { m_VerticalCollider.x, m_VerticalCollider.y, m_VerticalCollider.w, m_VerticalCollider.h };


	return true;
}

void CGoombas::Destroy(void)
{
	m_pDyingCallback = nullptr;

#define DELETE_ANIMATOR(Animator) delete Animator; Animator = nullptr;

	DELETE_ANIMATOR(m_pAnimatorDying);
	DELETE_ANIMATOR(m_pAnimatorWalking);

#undef DELETE_ANIMATOR

	m_pCurrentAnimator = nullptr;

	CGameObject::Destroy();
}

void CGoombas::Kill(void)
{

}

void CGoombas::RenderDebug(void)
{

}

void CGoombas::Update(const float deltaTime)
{

}

void CGoombas::HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime)
{
}

void CGoombas::SyncCollider(void)
{
	m_Collider.x = m_Rectangle.x + m_ColliderOffset.x;
	m_Collider.y = m_Rectangle.y + m_ColliderOffset.y;
}

void CGoombas::ActivateAnimator(CAnimator* animator)
{
	if (m_pCurrentAnimator != animator)
	{
		m_pCurrentAnimator = animator;
		m_pCurrentAnimator->Reset();
	}
}
