#include "Pch.h"
#include "Timer.h"

void CTimer::Update(void)
{
	m_CurrentTime	= SDL_GetTicks();
	m_DeltaTime		= std::min((m_CurrentTime - m_PreviousTime) * 0.001, 0.25);
	m_PreviousTime	= m_CurrentTime;
}