#pragma once

class CApplication;

class CTransitionRenderer
{
public:

	 CTransitionRenderer(void);
	 CTransitionRenderer(CApplication* application, const SDL_FPoint& size);
	~CTransitionRenderer(void);

	void	Update(const float deltaTime);
	void	Render(void);

	void	StartTransition(void);

public:

	// Place getters and setters in their own public field

	void	SetColor(const SDL_Color& color)	{m_Color = color;}

	float	GetTransitionValue(void) const		{return m_TransitionValue;}

	void	SetSpeed(const float speed)			{m_FadeSpeed = std::max(speed, 0.1f);}

	bool	IsTransitioning(void) const			{return (m_State != EState::IDLE);}

private:

	enum EState
	{
		IDLE = 0,
		FADING_IN,
		FADING_OUT
	};

private:

	CApplication*	m_pApplication				= nullptr;

	SDL_Renderer*	m_pRenderer					= nullptr;

	SDL_FRect		m_Transition				= {0.0f, 0.0f, 0.0f, 0.0f};

	SDL_Color		m_Color						= {0, 0, 0, 255};

	float			m_TransitionValue			= 1.0f;
	float			m_TransitionDelayDefault	= 0.3f;
	float			m_TransitionDelay			= m_TransitionDelayDefault;
	float			m_FadeSpeed					= 1.0f;

	EState			m_State						= EState::FADING_IN;

};