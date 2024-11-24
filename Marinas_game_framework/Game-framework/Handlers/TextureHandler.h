#pragma once

#include "Utilities/Texture.h"

class CApplication;

class CTextureHandler
{
public:

	CTextureHandler(void) {}
	CTextureHandler(CApplication* application) : m_pApplication(application) {}
	~CTextureHandler(void) {}

	bool		Create(const std::string& basePath);
	void		Destroy(void);

	CTexture* CreateTexture(const std::string& fileName);
	CTexture* CreateTextureFromSurface(SDL_Surface* surface, const std::string& name);
	CTexture* CreateEmptyTexture(const SDL_Point& size, const SDL_TextureAccess textureAccess, const std::string& name);

	void		DestroyTexture(const std::string& name);

private:

	CTexture* RetrieveTexture(const std::string& name);

private:

	typedef std::map<std::string, CTexture*> TextureMap;

private:

	CApplication* m_pApplication = nullptr;

	std::string		m_BasePath = "";

	TextureMap		m_TextureMap = {};

};