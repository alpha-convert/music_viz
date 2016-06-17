#pragma once
/***
* @Author Joseph Cutler
* @Date June 17, 2016
* @Copyright WTFPL
*/

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <cstdio>
#include <vector>

class Audio {
public:
	Audio();
	void SetBgMusic(const char* fname, const char* song_name = "");
	void StartBgMusic();
	void StopBgMusic();
	void PlayBgMusic();
	void PauseBgMusic();
	~Audio();
private:
	static constexpr unsigned int frequency = 44100;
	static constexpr unsigned int channels = 1;
	static constexpr unsigned int chunksize = 1024;

	Mix_Music *bg_music;
	const char* bg_music_fname;
	const char* bg_music_song_name;
	std::vector<Mix_Chunk *> all_chunks;

};
