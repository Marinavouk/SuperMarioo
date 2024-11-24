#pragma once

#include "Texture.h"

class CApplication;

class CWindow
{
public:

	CWindow(void) {}
	~CWindow(void) {}

	bool			Create(CApplication* application, const std::string& title, const bool fullscreen, const bool resizable);
	void			Destroy(void);

	bool			BeginRender(void);
	void			EndRender(void);

	bool			ClearBuffer();

	void			SetRenderTarget(CTexture* renderTarget);

	void			OnResized(void);

	void			ToggleFullscreen(void);

	void			ToggleResizable(void);

public:

	// Place getters and setters in their own public field

	SDL_Window* GetWindow(void) const { return m_pWindow; }

	SDL_Renderer* GetRenderer(void) const { return m_pRenderer; }

	SDL_FPoint& GetSize(void) const { return (SDL_FPoint&)m_Size; }
	SDL_FPoint& GetCenter(void) const { return (SDL_FPoint&)m_Center; }

	SDL_Color& GetClearColor(void) const { return (SDL_Color&)m_ClearColor; }
	void			SetClearColor(const SDL_Color& color) { m_ClearColor = color; }

	void			SetTitle(const std::string& title);

	bool			GetFullscreen(void) const;
	bool			GetResizable(void) const;

private:

	SDL_Window* m_pWindow = nullptr;
	SDL_Renderer* m_pRenderer = nullptr;

	SDL_FPoint		m_Size = { 0.0f, 0.0f };

	SDL_FPoint		m_Center = { 0.0f, 0.0f };

	SDL_Color		m_ClearColor = { 0, 0, 0, 255 }; // R, G, B, A (alpha/transparency)

};