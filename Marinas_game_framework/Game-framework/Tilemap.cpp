#include "Pch.h"
#include "Tilemap.h"



bool CTilemap::isSolid(int x, int y) const
{
    return data[y * width + x].solid;
}

CTilemap::CTilemap(CTextureHandler& textureHandler)
 {
    width = 12;
    height = 12;

    int initial_data[12][12] =
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    data.resize(width * height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            data[y * width + x].id = initial_data[y][x];
            data[y * width + x].solid = false; 
        }
    }

    groundBrick = textureHandler.CreateTexture("Assets/Textures/groundTile.png");
    blueBrick = textureHandler.CreateTexture("Assets/Textures/underGroundTile.png");
 }

   CTilemap::~CTilemap() = default;



void CTilemap::Render(SDL_Renderer* renderer)
{
    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            CTexture* textureToRender = nullptr;

            if (data[y * width + x].id == 0) 
            {
                textureToRender = groundBrick;
            }
            else if (data[y * width + x].id == 1) 
            {
                textureToRender = blueBrick;
            }

            if (textureToRender)
            {
                SDL_FPoint destinationRect
                {
                    x * tile_size,
                    y * tile_size
                };
                textureToRender->Render(destinationRect);
            }
        }
    }
}

void CTilemap::SetTile(int x, int y, int tile_id, bool solid)
{
    data[y * width + x].id = tile_id;
    data[y * width + x].solid = solid;
}


