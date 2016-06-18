#pragma once
/***
* @Author Joseph Cutler
* @Date June 17, 2016
* @Copyright WTFPL
*/

#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <stack>
#include <string>
#include <cassert>

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

#include "Graphics.h"
#include "Audio.h"
#include "Color.h"



class Visualizer {
public:
	Visualizer(Graphics *_g, const char* fname, const char *songname);
	~Visualizer();

	bool WaitingForWindowUpdate();
	void HandleEvent(SDL_Event e);
	void UpdateWindow();
	
private:
	Graphics *g;

	bool request_draw;
	bool request_update;
	static constexpr unsigned int frequency = 44100;
	static constexpr unsigned int channels = 1;
	static constexpr unsigned int chunksize = 1024;
	static constexpr unsigned int anim_frame_delay = 20;

	Mix_Music *song;
	const char *songname;
	const char *fname;

	SDL_TimerID callback_timer;

	static uint32_t ScreenUpdateRequestCallback(uint32_t interval, void* param);
	static void 	AudioDrawRequestCallback(void *udata, uint8_t *dstream, int len);
};
