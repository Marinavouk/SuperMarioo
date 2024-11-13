#pragma once

#include "GameObject.h"

class CPipe final : public CGameObject
{
public:

	CPipe(void) {}
	CPipe(CApplication* application) : CGameObject(application) {}
	~CPipe(void) {}

	virtual bool Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth) override;
	virtual void SetPosition(const SDL_FPoint& position) override;

private:

	SDL_FPoint m_ColliderOffset = { 10.0f, 10.0f };

};


