#pragma once

#include "Utilities/Texture.h"

class CApplication;

class CTilemap
{
public:

	CTilemap(void) {}
	~CTilemap(void) {}

	bool Create(CApplication* application);
	void Destroy(void);

	void Render(void);

private:

	enum ETileType
	{
		GROUND = 0,
	};

	struct STile
	{
		SDL_FPoint	m_Position = { 0.0f, 0.0f };
		SDL_Point	m_TexCoord = { 0, 0 };
	};

	typedef std::vector<STile>		TileVector;
	typedef std::vector<TileVector> TileVector2D;

private:

	CApplication* m_pApplication = nullptr;

	CTexture* m_pTexture = nullptr;

	TileVector2D	m_Tiles = {};

};