#pragma once
#include "Utilities/Texture.h"

struct Tile 
{
	int id;      
	bool solid;   
};

class CTilemap final 
{
public:

	CTilemap(void) {}

	~CTilemap(void) {}

	
public:

	void				Render();
	void				SetTile(int x, int y, int tile_id, bool solid);

	bool                isSolid(int x, int y);

public:

	CTexture* groundBrick= nullptr;
	CTexture* blueBrick= nullptr;

	int width = 12;	
	int height = 12; 
	Tile* data;

private: 
	 const int tile_size = 32;
};

