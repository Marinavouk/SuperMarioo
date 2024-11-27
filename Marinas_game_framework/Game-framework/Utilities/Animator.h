#pragma once

#include "Texture.h"

typedef std::function<void()> AnimationEndedCB;

class CAnimator
{
public:

	enum EDirection
	{
		FORWARD = 0,
		BACKWARD
	};

public:

	 CAnimator(void) {}
	~CAnimator(void) {}

	void		Set(CTexture* texture, const uint32_t numFrames, const uint32_t startFrame, const uint32_t endFrame, const uint32_t row, const SDL_FPoint& frameSize, const float speed, const bool loop, const EDirection direction);

	void		Update(const float deltaTime);

	void		Reset(void);

public:

	void		SetAnimationEndCallback(AnimationEndedCB callback) { m_pAnimationEndCallback = callback; }

	SDL_Rect&	GetClipRectangle(void) const { return (SDL_Rect&)m_ClipRectangle; }

	SDL_FPoint& GetFrameSize(void) const { return (SDL_FPoint&)m_FrameSize; }

	float		GetSpeed(void) const { return m_Speed; }
	void		SetSpeed(const float speed) { m_Speed = speed; }

	int32_t		GetCurrentFrame(void) const { return m_CurrentFrame; }

private:

	void		SetClipRectangle(void);

private:

	CTexture*			m_pTexture = nullptr;

	AnimationEndedCB	m_pAnimationEndCallback = nullptr;

	SDL_Rect			m_ClipRectangle = { 0, 0, 0, 0 };

	SDL_FPoint			m_FrameSize = { 0.0f, 0.0f };

	float				m_Speed = 0.0f;
	float				m_Advance = 0.0f;

	uint32_t			m_NumFrames = 0;
	uint32_t			m_StartFrame = 0;
	uint32_t			m_EndFrame = 0;
	uint32_t			m_Row = 0;

	int32_t				m_CurrentFrame = 0;

	bool				m_Loop = true;

	EDirection			m_Direction = EDirection::FORWARD;

};