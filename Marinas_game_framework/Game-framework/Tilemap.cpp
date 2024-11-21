#include "Pch.h"
#include "Tilemap.h"

CTilemap::CTilemap(CTextureHandler& textureHandler) 
{
    width = 12;
    height = 12;

    // Initial tilemap layout
    int initial_data[12][20] =
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    
    data.resize(width * height);
    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            data[y * width + x].id = initial_data[y][x];
            data[y * width + x].solid = (initial_data[y][x] == 1);
        }
    }

    groundBrick = textureHandler.CreateTexture("groundTile.png");
    blueBrick = textureHandler.CreateTexture("underGroundTile.png");

}

CTilemap::~CTilemap() 
{
    groundBrick = nullptr;
    blueBrick = nullptr;
}

void CTilemap::Render(SDL_Renderer* renderer)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            CTexture* textureToRender = nullptr;

            switch (data[y * width + x].id)
            {
            case 0:
                textureToRender = nullptr; 
                break;
            case 1:
                textureToRender = blueBrick; 
                break;
            case 2:
                textureToRender = nullptr;
                break;
            default:
                break;
            }

            if (textureToRender)
            {
                SDL_FPoint destinationRect{
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
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        data[y * width + x].id = tile_id;
        data[y * width + x].solid = solid;
    }
}


bool CTilemap::isSolid(int x, int y) const 
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return data[y * width + x].solid;
    }
    return false;
}
