#include "Pch.h"
#include "Tilemap.h"



bool CTilemap::isSolid(int x, int y)
{
    return data[y * width + x].solid;
}

CTilemap::CTilemap()
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

CTilemap::~CTilemap()
{
	delete[] data;  
}

void CTilemap::Render()
{
	for (int y = 0; y < width; y++) 
	{
		for (int x = 0; x < height; x++)
		{
			switch (x)
			{
			case 0: 
				break;

			case 1:
				break;

			default:
				break;
			}
		}
	}
}

void CTilemap::SetTile(int x, int y, int tile_id, bool solid)
{
    data[y * width + x].id = tile_id;
    data[y * width + x].id = solid;
}
