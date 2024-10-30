#include "Pch.h"
#include "LibraryHandler.h"

#include "Application.h"

bool CLibraryHandler::Create(CApplication* application)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		const std::string message = "Failed to initialize SDL. " + std::string(SDL_GetError());
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), application->GetWindow().GetWindow());

		return false;
	}

	if (!(IMG_Init(IMG_InitFlags::IMG_INIT_PNG) & IMG_InitFlags::IMG_INIT_PNG))
	{
		const std::string message = "Failed to initialize SDL_Image. " + std::string(IMG_GetError());
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), application->GetWindow().GetWindow());

		return false;
	}

	if (TTF_Init() == -1)
	{
		const std::string message = "Failed to initialize SDL_TTF. " + std::string(TTF_GetError());
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), application->GetWindow().GetWindow());

		return false;
	}

	if (!(Mix_Init(MIX_InitFlags::MIX_INIT_MP3 | MIX_InitFlags::MIX_INIT_OGG) & (MIX_InitFlags::MIX_INIT_MP3 | MIX_InitFlags::MIX_INIT_OGG)))
	{
		const std::string message = "Failed to initialize SDL_Mixer. " + std::string(Mix_GetError());
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), application->GetWindow().GetWindow());

		return false;
	}

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		const std::string message = "Failed to open audio device. " + std::string(Mix_GetError());
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), application->GetWindow().GetWindow());

		return false;
	}

	// If you have some other external library/libraries that you need to create/start up, you can do it here



	return true;
}

void CLibraryHandler::Destroy(void)
{
	// If you have some other external library/libraries that you have created/started up in the 'Create' function above, you should destroy/shut it down here



	Mix_Quit();	
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}