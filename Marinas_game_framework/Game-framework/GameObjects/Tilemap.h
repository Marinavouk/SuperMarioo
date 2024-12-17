#pragma once

#include "Utilities/Texture.h"

class CTilemap
{
public:

	typedef std::vector<SDL_FRect> TileColliders;

public:

	 CTilemap(void) {}
	~CTilemap(void) {}

	bool			Create(CApplication* application);
	void			Destroy(void);

	void			Render(void);
	void			RenderDebug(void);

public:

	SDL_FPoint&		GetTileSize(void) const { return (SDL_FPoint&)m_TileSize; }

	TileColliders&	GetColliders(void) const { return (TileColliders&)m_TileColliders; }

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

	CApplication*	m_pApplication = nullptr;

	CTexture*		m_pTexture = nullptr;

	SDL_FPoint		m_TileSize = { 32.0f, 32.0f };

	TileVector2D	m_Tiles = {};

	TileColliders	m_TileColliders = {};

};