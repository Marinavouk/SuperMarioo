#include "Pch.h"
#include "Window.h"

#include "Application.h"

bool CWindow::Create(CApplication* application, const std::string& title, const bool fullscreen, const bool resizable)
{
	SDL_Point windowSize = { 1024, 768 };

	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0)
	{
		const float desiredAspectRatio = 4.0f / 3.0f;
		const float maxHeightPercentage = 0.75f;

		const float windowHeight = ((float)displayMode.h * maxHeightPercentage);
		const float windowWidth = desiredAspectRatio * windowHeight;

		if (windowWidth > (float)displayMode.w)
		{
			windowSize.x = displayMode.w;
			windowSize.y = (int32_t)((float)displayMode.w / desiredAspectRatio);
		}

		else
		{
			windowSize.x = (int32_t)windowWidth;
			windowSize.y = (int32_t)windowHeight;
		}
	}

	else
	{
		const std::string message = "Failed to retrieve the display mode. " + std::string(SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), application->GetWindow().GetWindow());
	}

	Uint32 flags = SDL_WINDOW_SHOWN;

	if (fullscreen)	flags |= SDL_WINDOW_FULLSCREEN;
	if (resizable)	flags |= SDL_WINDOW_RESIZABLE;

	m_pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, flags);
	if (!m_pWindow)
	{
		const std::string message = "Failed to create SDL window. " + std::string(SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), application->GetWindow().GetWindow());

		return false;
	}

	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_pRenderer)
	{
		const std::string message = "Failed to create SDL renderer. " + std::string(SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), application->GetWindow().GetWindow());

		return false;
	}

	if (SDL_SetRenderDrawColor(m_pRenderer, m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a) < 0)
	{
		const std::string message = "Failed to set render draw color. " + std::string(SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_WARNING, "Warning", message.c_str(), application->GetWindow().GetWindow());
	}

	if (SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND) < 0)
	{
		const std::string message = "Failed to set render draw blend mode. " + std::string(SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_WARNING, "Warning", message.c_str(), application->GetWindow().GetWindow());
	}

	m_Size = { (float)windowSize.x, (float)windowSize.y };

	m_Center = { m_Size.x * 0.5f, m_Size.y * 0.5f };

	if (BeginRender())
		EndRender();

	SDL_WarpMouseInWindow(m_pWindow, windowSize.x / 2, windowSize.y / 2);

	return true;
}
void CWindow::Destroy(void)
{
	SDL_DestroyRenderer(m_pRenderer);
	m_pRenderer = nullptr;

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
}

bool CWindow::BeginRender(void)
{
	return ((SDL_SetRenderDrawColor(m_pRenderer, m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a) == 0) && ClearBuffer());
}

void CWindow::EndRender(void)
{
	SDL_RenderPresent(m_pRenderer);
}

bool CWindow::ClearBuffer(void)
{
	return (SDL_RenderClear(m_pRenderer) == 0);
}

void CWindow::SetRenderTarget(CTexture* renderTarget)
{
	if (!renderTarget)
	{
		SDL_SetRenderTarget(m_pRenderer, nullptr);

		return;
	}

	SDL_SetRenderTarget(m_pRenderer, renderTarget->GetTexture());
}

void CWindow::OnResized(void)
{
	int32_t windowWidth = 0;
	int32_t windowHeight = 0;
	SDL_GetWindowSize(m_pWindow, &windowWidth, &windowHeight);

	m_Size = { (float)windowWidth, (float)windowHeight };

	m_Center = { m_Size.x * 0.5f, m_Size.y * 0.5f };
}

void CWindow::ToggleFullscreen(void)
{
	SDL_SetWindowFullscreen(m_pWindow, (GetFullscreen() ? 0 : SDL_WindowFlags::SDL_WINDOW_FULLSCREEN_DESKTOP));
}

void CWindow::ToggleResizable(void)
{
	SDL_SetWindowResizable(m_pWindow, (GetResizable() ? SDL_FALSE : SDL_TRUE));
}

void CWindow::SetTitle(const std::string& title)
{
	SDL_SetWindowTitle(m_pWindow, title.c_str());
}

bool CWindow::GetFullscreen(void) const
{
	return (SDL_GetWindowFlags(m_pWindow) & SDL_WindowFlags::SDL_WINDOW_FULLSCREEN_DESKTOP);
}

bool CWindow::GetResizable(void) const
{
	return (SDL_GetWindowFlags(m_pWindow) & SDL_WindowFlags::SDL_WINDOW_RESIZABLE);
}