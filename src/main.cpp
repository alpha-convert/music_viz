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
#include "Visualizer.h"

constexpr unsigned int w = 1920/2;
constexpr unsigned int h = 1080/2;

/*
typedef struct VisData{
	bool request_draw;
	bool request_update;
	Graphics *g_ptr;
} VisData;

//S
uint32_t ScreenUpdateRequestCallback(Uint32 interval, void *param)
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

//https://www.libsdl.org/projects/SDL_mixer/docs/demos/sdlwav.c
//What evil person writes code like that
void GraphicsCallback(void *udata, uint8_t *dstream, int len){
	//Grab some stuff
	auto data = (VisData *)udata;
	auto g = data->g_ptr;
	auto request_draw= data->request_draw;

	uint16_t *raw = (uint16_t *) dstream;
	len = len/2;

	//If we have g, and there's a request to draw, draw the stuff
	if(g != nullptr && request_draw){
		auto w = g->width;
		auto h = g->height;
		for(int index = 1; index < len; index = index + 2){
			int8_t prev_val = dstream[index - 2];
			int8_t new_val = dstream[index];

			float ppa = static_cast<float>(index - 2)/static_cast<float>(len);
			float npa = static_cast<float>(index)/static_cast<float>(len);

			g->Line(w * ppa, prev_val + h/2,w * npa, new_val + h/2,Color::Black);
		}
	}
	//The draw request has been satisfied, and now we signal that the new draw needs to be updated to the screen
	data->request_draw = false;
	data->request_update= true;
}
*/

int main(int argc, char** argv){
	const char *name = argv[1] ? argv[1] : "No song";
	Graphics g(w,h,name);

	Visualizer vis(&g,name,name);

	SDL_Event e;
	bool running =  true;
	while(running){
		if(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT || e.window.event == SDL_WINDOWEVENT_CLOSE){
				running = false;
			}
			vis.HandleEvent(e);
	}
		if(vis.WaitingForWindowUpdate()){
			vis.UpdateWindow();
		}


	}

	SDL_Quit();
	return 0;
}
