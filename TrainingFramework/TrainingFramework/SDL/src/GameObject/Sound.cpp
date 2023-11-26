#include"Sound.h"
#include <SDL_ttf.h>

Sound::Sound()
{
    Init();
}

Sound::~Sound()
{
    CleanUp();
}

bool Sound::Init()
{
    bool ret = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) //SDL_INIT_AUDIO for Init SDL audio
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        ret = false;
        
    }
    //Initialize SDL_mixer
      // first argument sets the sound frequency, and 44100 is a standard frequency that works on most systems. The second argument determines the sample format, which again here we're using the default. The third argument is the number of hardware channels, and here we're using 2 channels for stereo. The last argument is the sample size, which determines the size of the chunks we use when playing sound. 2048 bytes (AKA 2 kilobyes) worked fine for me, but you may have to experiment with this value to minimize lag when playing sounds.
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        ret = false;
    }
    return ret;
}

void Sound::PlaySound()
{
    printf("PLAYING");
    //If there is no music playing
    if (Mix_PlayingMusic() == 0)
    {
        //Play the music
        Mix_PlayMusic(m_Music, -1); // The first argument is the music we want to play and the last argument is the number of times to repeat it. -1 for loop it util stop
    }
    //If music is being played
    else
    {
        //If the music is paused
        if (Mix_PausedMusic() == 1)
        {
            //Resume the music
            Mix_ResumeMusic();
        }
        //If the music is playing
        else
        {
            //Pause the music
            Mix_PauseMusic();
        }
    }
}

void Sound::PlaySoundEffect()
{
    Mix_PlayMusic(m_Music, 1);
}

void Sound::PauseSound()
{
    //Pause the music
    Mix_PauseMusic();
}

void Sound::ResumeSound()
{
    //Resume the music
    Mix_ResumeMusic();
}

void Sound::StopSound()
{
    //Stop the music
    Mix_HaltMusic();
}

void Sound::LoadSound(std::string pathMusic)
{
    //Load music
    m_Music = Mix_LoadMUS(pathMusic.c_str());
    if (m_Music == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        
    }
}


unsigned int Sound::LoadSfx(const char * path)
{
	unsigned int ret = 0;
	Mix_Chunk * chunk = Mix_LoadWAV(path);
	if (chunk == nullptr)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	else
	{
		sfxs.push_back(std::move(chunk));
		ret = sfxs.size() - 1;
	}
	return ret;
}

void Sound::PlaySfx(unsigned int sfx, int repeat)
{
	if (sfx < sfxs.size())
	{
		Mix_PlayChannel(-1, sfxs[sfx], repeat);
	}
}

void Sound::CleanUp()
{
    //Free Music
	for (auto it = sfxs.begin(); it != sfxs.end(); it++)
	{
		Mix_FreeChunk(*it);
	}
	sfxs.clear();
    Mix_FreeMusic(m_Music);
    m_Music = NULL;
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
