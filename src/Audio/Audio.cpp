#include "Audio.h"
#include <libkern/OSByteOrder.h>
Audio::Audio(){
	if (!SDL_WasInit(SDL_INIT_AUDIO)) {
		SDL_Init(SDL_INIT_AUDIO);
	}

	auto open_success = Mix_OpenAudio(frequency,AUDIO_S16SYS,channels,chunksize);
	if(open_success <  0){
		printf("Error opening audio: %s\n",Mix_GetError());
	}
	
}


void Audio::SetBgMusic(const char* fname, const char* song_name){
	bg_music_fname = fname;
	bg_music_song_name = song_name;

	bg_music = Mix_LoadMUS(fname);
}

void Audio::StartBgMusic(){
	if(!Mix_PlayingMusic()){
		Mix_PlayMusic(bg_music,-1);
	}
}

void Audio::PlayBgMusic(){
	Mix_ResumeMusic();
}

void Audio::PauseBgMusic(){
	Mix_PauseMusic();
}

void Audio::StopBgMusic(){
	Mix_HaltMusic();
}

Audio::~Audio(){
	for(auto chunk : all_chunks){
		Mix_FreeChunk(chunk);
		chunk = nullptr;
	}
	Mix_FreeMusic(bg_music);
	bg_music = nullptr;

	Mix_CloseAudio();
	Mix_Quit();
}
