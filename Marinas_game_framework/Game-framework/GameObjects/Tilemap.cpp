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

void CTilemap::Destroy(void)
{
	m_Tiles.clear();

	m_pApplication->GetTextureHandler().DestroyTexture(m_pTexture->GetName());
	m_pTexture = nullptr;

	m_pApplication = nullptr;
}

void CTilemap::Render(void)
{
	/*
	for (int i = 0; i < m_Tiles.size(); ++i)
	{
		for (int j = 0; j < m_Tiles[i].size(); ++j)
		{

		}
	}
	*/

	// Range-based for loop
	// This works the same as the outer-most for-loop commented out above
	for (const TileVector& row : m_Tiles)
	{
		// This works the same as the inner-most for-loop commented out above
		for (const STile& tile : row)
		{
			const SDL_FRect	rectangle = { tile.m_Position.x, tile.m_Position.y, m_TileSize.x, m_TileSize.y };
			const SDL_Rect	clipRectangle = { tile.m_TexCoord.x, tile.m_TexCoord.y, (int)m_TileSize.x, (int)m_TileSize.y };

			m_pTexture->SetTextureCoords(clipRectangle);
			m_pTexture->Render(tile.m_Position, &rectangle);
		}
	}
}

void CTilemap::RenderDebug(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	for (const SDL_FRect& collider : m_TileColliders)
	{
		SDL_RenderDrawRectF(renderer, &collider);
	}
}