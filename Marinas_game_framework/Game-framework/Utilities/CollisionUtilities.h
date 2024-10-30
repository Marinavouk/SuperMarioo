#pragma once

// Returns true if the two passed in SDL_FRect's are colliding/intersecting, false otherwise
inline bool RectVsRect(const SDL_FRect& quad1, const SDL_FRect& quad2, SDL_FRect* intersection = nullptr)
{
	SDL_FRect	Intersection	= {0.0f, 0.0f, 0.0f, 0.0f};
	const bool	Result			= (SDL_IntersectFRect(&quad1, &quad2, &Intersection) == SDL_TRUE);

	if (intersection)
	{
		intersection->x = Intersection.x;
		intersection->y = Intersection.y;
		intersection->w = Intersection.w;
		intersection->h = Intersection.h;
	}

	return Result;
}

// Returns true if the passed in point is colliding/intersecting with the passed in rectangle, false otherwise
inline bool PointInRect(const SDL_FPoint& point, const SDL_FRect& rect)
{
	return (SDL_PointInFRect(&point, &rect) == SDL_TRUE);
}