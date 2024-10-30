#include "Pch.h"
#include "Animator.h"

void CAnimator::Set(CTexture* texture, const uint32_t numFrames, const uint32_t startFrame, const uint32_t endFrame, const uint32_t row, const SDL_FPoint& frameSize, const float speed, const bool loop, const EDirection direction)
{
	m_pTexture		= texture;
	m_FrameSize		= frameSize;
	m_Speed			= speed;
	m_NumFrames		= numFrames;
	m_StartFrame	= startFrame;
	m_EndFrame		= endFrame;
	m_Row			= row;
	m_CurrentFrame	= startFrame;
	m_Loop			= loop;
	m_Direction		= direction;

	Reset();

	m_ClipRectangle.w = (uint32_t)m_FrameSize.x;
	m_ClipRectangle.h = (uint32_t)m_FrameSize.y;
}

void CAnimator::Update(const float deltaTime)
{
	m_Advance += m_Speed * deltaTime;

	if (m_Advance > 1.0f)
	{
		const bool Forward = (m_Direction == EDirection::FORWARD);

		m_CurrentFrame += (Forward ? 1 : -1);

		if (Forward)
		{
			if (m_CurrentFrame > (int32_t)m_EndFrame)
			{
				m_CurrentFrame = (m_Loop ? m_StartFrame : m_EndFrame);

				if(m_pAnimationEndCallback)
					m_pAnimationEndCallback();
			}
		}

		else
		{
			if (m_CurrentFrame < (int32_t)m_EndFrame)
			{
				m_CurrentFrame = (m_Loop ? m_StartFrame : m_EndFrame);

				if(m_pAnimationEndCallback)
					m_pAnimationEndCallback();
			}
		}

		SetClipRectangle();

		if (m_pTexture)
			m_pTexture->SetTextureCoords(m_ClipRectangle);

		m_Advance = 0.0f;
	}
}

void CAnimator::Reset(void)
{
	m_Advance = 0.0f;

	m_CurrentFrame = m_StartFrame;

	SetClipRectangle();
}

void CAnimator::SetClipRectangle(void)
{
	m_ClipRectangle.x = (uint32_t)m_FrameSize.x	* m_CurrentFrame;
	m_ClipRectangle.y = (uint32_t)m_FrameSize.y	* m_Row;
}