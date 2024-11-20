#pragma once
#include "Utilities/Texture.h"


class CTilemap
{

public:
	CTilemap(void) {}
	~CTilemap(void) {}

public:
	void				Render();
	void				SetTile(int x, int y, int tile_id, bool solid);

	bool                isSolid(int x, int y);

public:

	int width = 12;	//in tiles
	int height = 12; // in pixels
	

private:
	const int tile_size = 32;

};

