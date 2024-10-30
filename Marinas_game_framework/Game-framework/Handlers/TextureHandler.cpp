#include "Pch.h"
#include "TextureHandler.h"

#include "Application.h"
#include "Utilities/StringUtilities.h"

bool CTextureHandler::Create(const std::string& basePath)
{
	m_BasePath = basePath;

	CTexture* defaultTexture = CreateTexture("default.png");

	if(!defaultTexture)
		return false;

	m_TextureMap["default"] = defaultTexture;

	return true;
}

void CTextureHandler::Destroy(void)
{
	DestroyTexture("default");

	m_TextureMap.clear();

	m_pApplication = nullptr;
}

CTexture* CTextureHandler::CreateTexture(const std::string& fileName)
{
	if(fileName.empty())
		return RetrieveTexture("default");

	const std::string name = RemoveFileExtension(fileName);

	CTexture* texture = RetrieveTexture(name);

	if (texture)
		return texture;

	texture = new CTexture;

	if(!texture->Create(m_pApplication, m_pApplication->GetWindow().GetRenderer(), m_BasePath + "/" + fileName))
	{
		delete texture;
		texture = nullptr;

		return RetrieveTexture("default");
	}

	texture->m_ReferenceCount++;

	m_TextureMap[name] = texture;

	return texture;
}

CTexture* CTextureHandler::CreateTextureFromSurface(SDL_Surface* surface, const std::string& name)
{
	if (!surface)
	{
		const std::string message = "Failed to create texture from surface. Surface is nullptr.";
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), m_pApplication->GetWindow().GetWindow());

		return RetrieveTexture("default");
	}

	if (name.empty())
	{
		const std::string message = "Failed to create texture from surface. Name is empty.";
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), m_pApplication->GetWindow().GetWindow());

		return RetrieveTexture("default");
	}

	CTexture* texture = RetrieveTexture(name);

	if (texture)
		return texture;

	texture = new CTexture;

	if(!texture->CreateFromSurface(m_pApplication, m_pApplication->GetWindow().GetRenderer(), surface, name))
	{
		delete texture;
		texture = nullptr;

		return RetrieveTexture("default");
	}

	texture->m_ReferenceCount++;

	m_TextureMap[name] = texture;

	return texture;
}

CTexture* CTextureHandler::CreateEmptyTexture(const SDL_Point& size, const SDL_TextureAccess textureAccess, const std::string& name)
{
	if (name.empty())
	{
		const std::string message = "Failed to create empty texture. Name is empty.";
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), m_pApplication->GetWindow().GetWindow());

		return RetrieveTexture("default");
	}

	CTexture* texture = RetrieveTexture(name);

	if (texture)
		return texture;

	texture = new CTexture;

	if(!texture->CreateEmpty(m_pApplication, m_pApplication->GetWindow().GetRenderer(), size, textureAccess, name))
	{
		delete texture;
		texture = nullptr;

		return RetrieveTexture("default");
	}

	texture->m_ReferenceCount++;

	m_TextureMap[name] = texture;

	return texture;
}

void CTextureHandler::DestroyTexture(const std::string& name)
{
	if(name.empty())
		return;

	TextureMap::iterator FindIt = m_TextureMap.find(name);

	if(FindIt == m_TextureMap.end())
	{
		const std::string message = "Failed to destroy texture '" + name + "'. Texture not found";
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), m_pApplication->GetWindow().GetWindow());

		return;
	}

	if(FindIt->second->m_ReferenceCount > 0)
		FindIt->second->m_ReferenceCount--;

	if(FindIt->second->m_ReferenceCount != 0)
		return;

	FindIt->second->Destroy();
	delete FindIt->second;
	FindIt->second = nullptr;

	m_TextureMap.erase(FindIt);
}

CTexture* CTextureHandler::RetrieveTexture(const std::string& name)
{
	TextureMap::const_iterator FindIt = m_TextureMap.find(name);

	if(FindIt != m_TextureMap.end())
	{
		FindIt->second->m_ReferenceCount++;

		return FindIt->second;
	}

	return nullptr;
}