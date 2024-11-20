#pragma once
#include "Utilities/Texture.h"


struct Tile 
{
	int id;      
	bool solid;   
};

class CTilemap
{
public:

	CTilemap(void) {}
	~CTilemap(void) {}

public:

	void				Render();
	void				SetTile(int x, int y, int tile_id, bool solid);
	void				Map(int width, int height);

	bool                isSolid(int x, int y);

public:

	int width = 12;	
	int height = 12; 
	Tile* data;


private: 
	 const int tile_size = 32;
};

