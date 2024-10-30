#pragma once

#include "Utilities/Texture.h"

class CApplication;
class CInputHandler;

class CButton
{
public:

	 CButton(void) {}
	~CButton(void) {}

	bool		Create(CApplication* application, TTF_Font* font, const std::string& text, const SDL_Color& textColor);
	void		Destroy(CApplication* application);

	void		Update(CInputHandler& inputHandler);
	void		Render(SDL_Renderer* renderer, const SDL_FPoint* mousePosition = nullptr);

	bool		IsPressed(CInputHandler& inputHandler);

public:

	// Place getters and setters in their own public field

	SDL_FPoint&	GetPosition(void) const								{return (SDL_FPoint&)m_Position;}
	void		SetPosition(const SDL_FPoint& newPosition);

	SDL_FPoint	GetSize(void) const									{return {m_TextRectangle.w, m_TextRectangle.h};}

	void		SetBackgroundColor(const SDL_Color& color)			{m_BackgroundColor = color;}
	void		SetBackgroundPressedColor(const SDL_Color& color)	{m_BackgroundColorPressed = color;}
	void		SetBackgroundDisabledColor(const SDL_Color& color)	{m_BackgroundColorDisabled = color;}
	void		SetTextColor(const SDL_Color& color)				{m_TextColor = color;}
	void		SetTextColorHovered(const SDL_Color& color)			{m_TextColorHovered = color;}
	void		SetTextColorPressed(const SDL_Color& color)			{m_TextColorPressed = color;}

	void		SetTriggerButton(const int32_t newTriggerButton)	{m_TriggerButton = newTriggerButton;}

	bool		GetEnabled(void) const								{return m_Enabled;}
	void		SetEnabled(const bool enabled)						{m_Enabled = enabled;}

private:

	bool		PointInside(const SDL_FPoint& point);

private:

	CTexture*	m_pTexture					= nullptr;

	SDL_FPoint	m_Position					= {0.0f, 0.0f};

	SDL_FRect	m_TextRectangle				= {0.0f, 0.0f, 0.0f, 0.0f};

	SDL_Color	m_BackgroundColor			= {0,	0,		0,		255};
	SDL_Color	m_BackgroundColorPressed	= {0,	0,		0,		255};
	SDL_Color	m_BackgroundColorDisabled	= {0,	0,		0,		255};
	SDL_Color	m_TextColor					= {255, 255,	255,	255};
	SDL_Color	m_TextColorHovered			= {255, 255,	255,	255};
	SDL_Color	m_TextColorPressed			= {255, 255,	255,	255};
	SDL_Color	m_TextColorDisabled			= {100, 100,	100,	255};

	float		m_ScaleDefault				= 1.0f;
	float		m_ScalePressed				= 1.1f;
	float		m_CurrentScale				= m_ScaleDefault;

	int32_t		m_TriggerButton				= SDL_BUTTON_LEFT;

	bool		m_Held						= false;
	bool		m_Enabled					= true;

};