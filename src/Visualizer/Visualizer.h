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
	Visualizer(Graphics *_g, Audio *_a);
	~Visualizer();
private:
	Graphics *g;
	Audio *a;

	bool request_draw;
	bool request_update;

	static uint32_t ScreenUpdateRequestCallback(uint32_t interval, void* param);
	static void 	AudioDrawRequestUpdate(void *udata, uint8_t *dstream, int len);
};
