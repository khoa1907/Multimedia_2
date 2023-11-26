#pragma once
#include "SDL_mixer.h"
#include <string>
#include <vector>
class Sound
{
public:
	Sound();
	~Sound();
	bool Init();
	void PlaySound();
	void PlaySoundEffect();
	void PauseSound();
	void ResumeSound();
	void StopSound();
	void LoadSound(std::string pathMusic);
	unsigned int LoadSfx(const char * path);
	void PlaySfx(unsigned int sfx, int repeat = 0);
	void CleanUp();
private:
	std::string m_PathMusic;
	Mix_Music* m_Music = NULL;
	std::vector<Mix_Chunk*>	sfxs;
};
