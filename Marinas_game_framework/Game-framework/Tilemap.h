#pragma once

#include "Handlers/TextureHandler.h"
#include <vector>

struct Tile 
{
    int id;      
    bool solid;  
};

class CTilemap final 
{
public:
    CTilemap(CTextureHandler& textureHandler);
    ~CTilemap();

    void Render(SDL_Renderer* renderer);
    void SetTile(int x, int y, int tile_id, bool solid);
    

    bool isSolid(int x, int y) const;

private:
    const int tile_size = 64; 
    int width = 12;           
    int height = 12;          

    CTexture* groundBrick = nullptr; 
    CTexture* blueBrick = nullptr;   

    std::vector<Tile> data; 
};
