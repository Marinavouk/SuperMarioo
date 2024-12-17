#include "Pch.h"
#include "GameObject.h"


#include "Application.h"


bool CGameObject::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
	m_pTexture = m_pApplication->GetTextureHandler().CreateTexture(textureFileName);


	int32_t textureWidth = 0;
	int32_t textureHeight = 0;
	SDL_QueryTexture(m_pTexture->GetTexture(), nullptr, nullptr, &textureWidth, &textureHeight);


	m_Rectangle = { position.x, position.y, (float)textureWidth, (float)textureHeight };


	m_Collider = m_Rectangle;


	m_MaxHealth = maxHealth;
	m_CurrentHealth = maxHealth;


	return true;
}


void CGameObject::Destroy(void)
{
	m_pApplication->GetTextureHandler().DestroyTexture(m_pTexture->GetName());
	m_pTexture = nullptr;
}


void CGameObject::Kill(void)
{
	m_IsDead = true;
}


void CGameObject::SetPosition(const SDL_FPoint& position)
{
	m_Rectangle.x = position.x;
	m_Rectangle.y = position.y;


	m_Collider.x = m_Rectangle.x;
	m_Collider.y = m_Rectangle.y;
}


void CGameObject::Render(void)
{
	m_pTexture->SetFlipMethod(m_FlipMethod);
	m_pTexture->Render({ m_Rectangle.x, m_Rectangle.y });
}


void CGameObject::RenderDebug(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();


	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Rectangle);


	SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Collider);
}
