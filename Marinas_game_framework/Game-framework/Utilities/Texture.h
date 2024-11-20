#pragma once

class CApplication;

class CTexture
{
	friend class CTextureHandler;

public:

	 CTexture(void) {}
	~CTexture(void) {}

	bool				Create(CApplication* application, SDL_Renderer* renderer, const std::string& fileName);
	bool				CreateFromSurface(CApplication* application, SDL_Renderer* renderer, SDL_Surface* surface, const std::string& name);
	bool				CreateEmpty(CApplication* application, SDL_Renderer* renderer, const SDL_Point& size, const SDL_TextureAccess textureAccess, const std::string& name);

	void				Destroy(void);

	void				Render(const SDL_FPoint& position, const SDL_FRect* destinationRectangle = nullptr);

	void				SetTextureCoords(const uint32_t left, const uint32_t right, const uint32_t top, const uint32_t bottom);
	void				SetTextureCoords(const SDL_Rect& clipRectangle);

	void				SetColorMod(const Uint8 r, const Uint8 g, const Uint8 b);
	void				SetColorMod(const SDL_Color& color);

	void				SetAlphaMod(const Uint8 alpha);

public:

	SDL_Texture*		GetTexture(void) const								{return m_pTexture;}

	SDL_Rect*			GetClipRectangle(void) const						{return m_pClipRectangle;}

	SDL_RendererFlip	GetFlipMethod(void)	const							{return m_FlipMethod;}
	void				SetFlipMethod(const SDL_RendererFlip flipMethod)	{m_FlipMethod = flipMethod;} //

	SDL_FPoint&			GetSize(void) const									{return (SDL_FPoint&)m_Size;}
	void				SetSize(const SDL_FPoint& size)						{m_Size = size;}

	float				GetAngle(void) const								{return m_Angle;}
	void				SetAngle(const float angle)							{m_Angle = angle;}

	std::string&		GetName(void) const									{return (std::string&)m_Name;}

private:

	SDL_Renderer*		m_pRenderer			= nullptr;

	SDL_Texture*		m_pTexture			= nullptr;

	SDL_Rect*			m_pClipRectangle	= nullptr;

	SDL_RendererFlip	m_FlipMethod		= SDL_RendererFlip::SDL_FLIP_NONE;

	SDL_FPoint			m_Size				= {0.0f, 0.0f};

	float				m_Angle				= 0.0f;

	uint32_t			m_ReferenceCount	= 0;

	std::string			m_Name				= "";

};