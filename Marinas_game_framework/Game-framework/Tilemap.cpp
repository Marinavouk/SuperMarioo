#include "Pch.h"
#include "Tilemap.h"



bool CTilemap::isSolid(int x, int y)
{
    return data[y * width + x].solid;
}

 void CTilemap::Map(int width, int height)
 {
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0;
 }

void CTilemap::Render()
{
	for (int y = 0; y < width; y++) 
	{
		for (int x = 0; x < height; x++)
		{
			//
		}
	}
}

void CTilemap::SetTile(int x, int y, int tile_id, bool solid)
{
    data[y * width + x].id = tile_id;
    data[y * width + x].id = solid;
}
