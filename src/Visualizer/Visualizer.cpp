#include "Visualizer.h"
Visualizer::Visualizer(Graphics *g, const char* fname, const char *songname){
	this->g = g;
	this->fname = fname;
	this->songname = songname;

	request_draw = false;
	request_update = false;

	if (!SDL_WasInit(SDL_INIT_AUDIO)) {
		SDL_Init(SDL_INIT_AUDIO);
	}

	auto open_success = Mix_OpenAudio(frequency,AUDIO_S16SYS,channels,chunksize);
	if(open_success <  0){
		printf("Error opening audio: %s\n",Mix_GetError());
	}

	song = Mix_LoadMUS(fname);

	if(!Mix_PlayingMusic()){
		Mix_PlayMusic(song,-1);
	}

	callback_timer = SDL_AddTimer(anim_frame_delay, this->ScreenUpdateRequestCallback, (void *)this);
	Mix_SetPostMix(this->AudioDrawRequestCallback, (void *)this);

}
Visualizer::~Visualizer(){
	Mix_CloseAudio();
	Mix_Quit();
}

uint32_t Visualizer::ScreenUpdateRequestCallback(uint32_t interval, void* param){
	auto _this = (Visualizer *) param;
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

void Visualizer::AudioDrawRequestCallback(void *udata, uint8_t *dstream, int len){
	Visualizer * _this = (Visualizer *)udata;
	//Grab some stuff
	//len = len/2;

	//If we have g, and there's a request to draw, draw the stuff
	if(_this->g != nullptr && _this->request_draw){
		auto g = _this->g;
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
	_this->request_draw = false;
	_this->request_update= true;
}


bool Visualizer::WaitingForWindowUpdate(){
	return request_update;
}

void Visualizer::UpdateWindow(){
	g->Clear();
	g->Update();
	request_update = false;
}

void Visualizer::HandleEvent(SDL_Event e){
	if(e.type == SDL_USEREVENT){
		//Bing bing bing! Timer went off.
		//Can I get a draw please?
		request_draw = true;
	}
}



