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
#include "Color.h"
#include "Visualizer.h"

constexpr unsigned int w = 2*1920/3;
constexpr unsigned int h = 2*1080/3;

int main(int argc, char** argv){
	const char *name = argv[1] ? argv[1] : "No song";
	Graphics g(w,h,name);

	Visualizer vis(&g,name);

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
