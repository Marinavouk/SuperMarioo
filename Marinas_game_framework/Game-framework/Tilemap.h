#pragma once
#include "Handlers/TextureHandler.h"

struct Tile 
{
	int id;      
	bool solid;   
};

class CTilemap final 
{
public:

	CTilemap(CTextureHandler& textureHandler) {}

	~CTilemap(void) {}

	
public:

	void				Render(SDL_Renderer* renderer);
	void				SetTile(int x, int y, int tile_id, bool solid);


	bool isSolid(int x, int y) const;

public:

	CTexture* groundBrick= nullptr;
	CTexture* blueBrick= nullptr;

	int width = 12;	
	int height = 12; 

	std::vector<Tile> data;


private: 
	 const int tile_size = 32;
};

