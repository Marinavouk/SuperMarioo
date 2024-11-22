#include "Pch.h"
#include "Pipe.h"

#include "Application.h"

bool CPipe::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	const SDL_FPoint textureSize = m_pTexture->GetSize();
	const float scaleFactor = 0.5f;
	const SDL_FPoint newTextureSize = { textureSize.x * scaleFactor, textureSize.y * scaleFactor };

	m_pTexture->SetSize(newTextureSize);

	m_Rectangle = { position.x, position.y, newTextureSize.x, newTextureSize.y };

	m_Collider = { m_Rectangle.x + m_ColliderOffset.x, m_Rectangle.y + m_ColliderOffset.y, newTextureSize.x - 30.0f, 50.0f };

	return true;
}

void CPipe::SetPosition(const SDL_FPoint& position)
{
	CGameObject::SetPosition(position);

	m_Collider.x = m_Rectangle.x + m_ColliderOffset.x;
	m_Collider.y = m_Rectangle.y + m_ColliderOffset.y;
}

void CPipe::RenderDebug(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Collider);
}

