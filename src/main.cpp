#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <stack>
#include <string>
#include <cassert>

#include <SDL2_mixer/SDL_mixer.h>

#include "Graphics.h"
#include "Audio.h"
#include "Color.h"

constexpr unsigned int w = 1920/2;
constexpr unsigned int h = 1080/2;

typedef struct VisData{
	bool request_draw;
	bool request_update;
	Graphics *g_ptr;
} VisData;

Uint32 ScreenUpdateRequestCallback(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return(interval);
}

void GraphicsCallback(void *udata, uint8_t *dstream, int len){
	auto data = (VisData *)udata;
	auto g = data->g_ptr;
	auto request_draw= data->request_draw;

	uint8_t prev_val;

	if(g != nullptr && request_draw){
		for(int index = 1; index < (len - 1); ++index){
			prev_val = dstream[index - 1];
			uint16_t new_val = dstream[index];

			float pos_along = static_cast<float>(index)/static_cast<float>(len);

			g->PutPixel(g->width * pos_along,g->height/2 - new_val,Color::Black);
		}
	}
	data->request_draw = false;
	data->request_update= true;
}

int main(int argc, char** argv){
	auto name = "assets/watchtower.wav";
	//auto name = argv[1] ? argv[1] : "WIndow";
	Graphics g(w,h,name);
	Audio a;

	a.SetBgMusic(name);
	a.StartBgMusic();

	VisData vis;
	vis.g_ptr = &g;
	vis.request_update = false;
	vis.request_draw = false;

	uint32_t anim_frame_delay = 10;  /* To round it down to the nearest 10 ms */
	auto callback_timer = SDL_AddTimer(anim_frame_delay, ScreenUpdateRequestCallback, (void*)&vis);

	Mix_SetPostMix(GraphicsCallback, (void *)&vis);


	uint16_t angle = 0;


	SDL_Event e;
	bool running =  true;
	while(running){
		if(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT || e.window.event == SDL_WINDOWEVENT_CLOSE){
				running = false;
			}
			if(e.type == SDL_USEREVENT){
				vis.request_draw = true;
				if(vis.request_update){
					g.Clear();
					g.Update();
					vis.request_update = false;
				}
			}
		}

	}

	SDL_Quit();
	return 0;
}
