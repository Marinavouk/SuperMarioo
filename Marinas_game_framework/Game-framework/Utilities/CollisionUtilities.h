#pragma once

inline bool QuadVsQuad(const SDL_FRect& rQuad1, const SDL_FRect& rQuad2, SDL_FRect* pIntersection = nullptr)
{
	SDL_FRect	Intersection = { 0.0f, 0.0f, 0.0f, 0.0f };
	const bool	Result = (SDL_IntersectFRect(&rQuad1, &rQuad2, &Intersection) == SDL_TRUE);

	if (pIntersection)
	{
		pIntersection->x = Intersection.x;
		pIntersection->y = Intersection.y;
		pIntersection->w = Intersection.w;
		pIntersection->h = Intersection.h;
	}

	return Result;
}

// Returns true if the passed in point is colliding/intersecting with the passed in rectangle, false otherwise
inline bool PointInRect(const SDL_FPoint& point, const SDL_FRect& rect)
{
	return (SDL_PointInFRect(&point, &rect) == SDL_TRUE);
}