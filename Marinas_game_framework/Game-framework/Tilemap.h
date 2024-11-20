#pragma once
#include "Utilities/Texture.h"


class CTilemap
{
	struct Tile 
	{
		int id;      
		bool solid;   
	};

public:

	CTilemap(void) {}
	~CTilemap(void) {}

public:

	void				Render(SDL_Renderer* renderer, CTexture* texture);
	void				SetTile(int x, int y, int tile_id, bool solid);

	bool                isSolid(int x, int y);

public:

	int width = 12;	//in tiles
	int height = 12; // in pixels
	const int tile_size = 32;
	

private:

	std::vector<std::vector<Tile>> tiles;
};

