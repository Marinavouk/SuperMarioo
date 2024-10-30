#include "Pch.h"
#include "AudioHandler.h"

#include "Application.h"

Mix_Chunk* CAudioHandler::CreateSound(const std::string& fileName)
{
	Mix_Chunk* chunk = Mix_LoadWAV(fileName.c_str());

	if (!chunk)
	{
		const std::string message = "Failed to load sound file '" + fileName + "'. " + Mix_GetError();
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), m_pApplication->GetWindow().GetWindow());
	}

	return chunk;
}

Mix_Music* CAudioHandler::CreateMusic(const std::string& fileName)
{
	Mix_Music* music = Mix_LoadMUS(fileName.c_str());

	if (!music)
	{
		const std::string message = "Failed to load music file '" + fileName + "'. " + Mix_GetError();
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), m_pApplication->GetWindow().GetWindow());
	}

	return music;
}

void CAudioHandler::DestroySound(Mix_Chunk* chunk)
{
	if (!chunk)
		return;

	Mix_FreeChunk(chunk);
}

void CAudioHandler::DestroyMusic(Mix_Music* music)
{
	if (!music)
		return;

	Mix_FreeMusic(music);
}

bool CAudioHandler::PlayMusic(Mix_Music* music, const int32_t numLoops)
{
	if (!music)
		return false;

	if (Mix_PlayMusic(music, numLoops) == -1)
	{
		const std::string message = "Failed to play music. " + std::string(Mix_GetError());
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), m_pApplication->GetWindow().GetWindow());

		return false;
	}

	return true;
}

bool CAudioHandler::StopMusic()
{
	if (Mix_PlayingMusic() == 0)
		return false;

	Mix_HaltMusic();

	return true;
}

void CAudioHandler::SetMusicVolume(const int32_t volume)
{
	if (volume < 0)
	{
		Mix_VolumeMusic(0);

		return;
	}

	Mix_VolumeMusic(volume);
}