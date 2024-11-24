#pragma once 

class CApplication;

class CAudioHandler
{
public:

	CAudioHandler(void) {}
	CAudioHandler(CApplication* application) : m_pApplication(application) {}
	~CAudioHandler(void) {}

	Mix_Chunk* CreateSound(const std::string& fileName);
	Mix_Music* CreateMusic(const std::string& fileName);

	void		DestroySound(Mix_Chunk* chunk);
	void		DestroyMusic(Mix_Music* music);

	// If parameter 'numLoops' is -1, the music will loop forever
	bool		PlayMusic(Mix_Music* music, const int32_t numLoops);
	bool		StopMusic();

	// Must be a value between 0 (complete silence) and MIX_MAX_VOLUME (128)
	void		SetMusicVolume(const int32_t volume);

private:

	CApplication* m_pApplication = nullptr;

};