#pragma once

class CTimer
{
public:

	 CTimer(void) {}
	~CTimer(void) {}

	void	Update();

public:

	// Place getters and setters in their own public field

	double	GetDeltaTime(void) const {return m_DeltaTime;}

private:

	double m_CurrentTime	= 0.0;
	double m_PreviousTime	= 0.0;
	double m_DeltaTime		= 0.0;

};

