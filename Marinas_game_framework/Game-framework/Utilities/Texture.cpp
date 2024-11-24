#include "Pch.h"
#include "Texture.h"

#include "Application.h"
#include "Handlers/TextureHandler.h"
#include "StringUtilities.h"

bool CTexture::Create(CApplication* application, SDL_Renderer* renderer, const std::string& filePath)
{
	m_pRenderer = renderer;

	m_pTexture = IMG_LoadTexture(renderer, filePath.c_str());

	if (!m_pTexture)
	{
		const std::string message = "Failed to create texture '" + GetFileName(filePath) + "'. " + IMG_GetError();
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), application->GetWindow().GetWindow());

		return false;
	}

	SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_BLEND);

	int32_t textureWidth = 0;
	int32_t textureHeight = 0;
	SDL_QueryTexture(m_pTexture, NULL, NULL, &textureWidth, &textureHeight);

	m_Size = { (float)textureWidth, (float)textureHeight };

	m_Name = RemoveFileExtension(GetFileName(filePath));

	return true;
}

bool CTexture::CreateFromSurface(CApplication* application, SDL_Renderer* renderer, SDL_Surface* surface, const std::string& name)
{
	m_pRenderer = renderer;

	m_pTexture = SDL_CreateTextureFromSurface(renderer, surface);

	if (!m_pTexture)
	{
		const std::string message = "Failed to create texture '" + name + "' from surface." + SDL_GetError();
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), application->GetWindow().GetWindow());

		return false;
	}

	SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_BLEND);

	int32_t textureWidth = 0;
	int32_t textureHeight = 0;
	SDL_QueryTexture(m_pTexture, NULL, NULL, &textureWidth, &textureHeight);

	m_Size = { (float)textureWidth, (float)textureHeight };

	m_Name = name;

	return true;
}

bool CTexture::CreateEmpty(CApplication* application, SDL_Renderer* renderer, const SDL_Point& size, const SDL_TextureAccess textureAccess, const std::string& name)
{
	m_pRenderer = renderer;

	m_pTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, textureAccess, size.x, size.y);

	if (!m_pTexture)
	{
		const std::string message = "Failed to create empty texture '" + name + "'. " + SDL_GetError();
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), application->GetWindow().GetWindow());

		return false;
	}

	SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_BLEND);

	m_Size = { (float)size.x, (float)size.y };

	m_Name = name;

	return true;
}

void CTexture::Destroy(void)
{
	delete m_pClipRectangle;
	m_pClipRectangle = nullptr;

	SDL_DestroyTexture(m_pTexture);
	m_pTexture = nullptr;
}

void CTexture::Render(const SDL_FPoint& position, const SDL_FRect* destinationRectangle)
{
	const SDL_FRect rectangle = { position.x, position.y, m_Size.x, m_Size.y };

	SDL_RenderCopyExF(m_pRenderer, m_pTexture, m_pClipRectangle, (destinationRectangle ? destinationRectangle : &rectangle), m_Angle, nullptr, m_FlipMethod);
}

void CTexture::SetTextureCoords(const uint32_t Left, const uint32_t Right, const uint32_t Top, const uint32_t Bottom)
{
	if (!m_pClipRectangle)
		m_pClipRectangle = new SDL_Rect;

	m_pClipRectangle->x = Left;
	m_pClipRectangle->y = Top;
	m_pClipRectangle->w = Right - Left;
	m_pClipRectangle->h = Bottom - Top;
}

void CTexture::SetTextureCoords(const SDL_Rect& clipRectangle)
{
	if (!m_pClipRectangle)
		m_pClipRectangle = new SDL_Rect;

	*m_pClipRectangle = clipRectangle;
}

void CTexture::SetColorMod(const Uint8 r, const Uint8 g, const Uint8 b)
{
	SDL_SetTextureColorMod(m_pTexture, r, g, b);
}

void CTexture::SetColorMod(const SDL_Color& color)
{
	SDL_SetTextureColorMod(m_pTexture, color.r, color.g, color.b);
}

void CTexture::SetAlphaMod(const Uint8 alpha)
{
	SDL_SetTextureAlphaMod(m_pTexture, alpha);
}