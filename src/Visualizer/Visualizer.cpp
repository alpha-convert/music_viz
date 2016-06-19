#include "Visualizer.h"
Visualizer::Visualizer(Graphics *g, const char* fname){
	this->g = g;
	this->fname = fname;
	this->songname = fname;

	request_draw = false;
	request_update = false;

	//Wew lad let's go
	if (!SDL_WasInit(SDL_INIT_AUDIO)) {
		SDL_Init(SDL_INIT_AUDIO);
	}

	auto open_success = Mix_OpenAudio(frequency,AUDIO_S16SYS,channels,chunksize);
	if(open_success <  0){
		printf("Error opening audio: %s\n",Mix_GetError());
	}

	//Load up the song!!
	song = Mix_LoadMUS(fname);

	//Play
	if(!Mix_PlayingMusic()){
		Mix_PlayMusic(song,-1);
	}

	//Set up the update request callback
	callback_timer = SDL_AddTimer(anim_frame_delay, this->ScreenUpdateRequestCallback, (void *)this);

	//Set up the viz draw request callback
	Mix_SetPostMix(this->AudioDrawRequestCallback, (void *)this);

	//Set the volume
	Mix_VolumeMusic(MIX_MAX_VOLUME);
	volume = Mix_VolumeMusic(-1);

}
Visualizer::~Visualizer(){
	Mix_CloseAudio();
	Mix_Quit();
}

uint32_t Visualizer::ScreenUpdateRequestCallback(uint32_t interval, void* param){
	auto _this = (Visualizer *) param;
	USE(_this);
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = UPDATE_CODE;
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

	//If we have g, and there's a request to draw, draw the stuff
	if(_this->g != nullptr && _this->request_draw){
		auto g = _this->g;
		auto w = g->width;
		auto h = g->height;
		//This format is so wierd
		int8_t prior_left = 0;
		int16_t prior_right = 0;
		for(int index = 1; index < len; index = index + 4){
			//Grab the value out of the left and right channels;
			int8_t left = *(dstream + index);
			int8_t right = *(dstream + index + 2);

			//Figure out how car along the screen we are
			float ppa = static_cast<float>(index - 4)/static_cast<float>(len);
			float npa = static_cast<float>(index)/static_cast<float>(len);

			//Left and right vertial offsets;
			auto left_va = h/4;
			auto right_va = 3*h/4;

			//Draw the line pieces.
			g->Line(w * ppa,
					prior_left + left_va,
					w * npa, 
					left + left_va,
					Color::Black);

			g->Line(w * ppa,
					prior_right + right_va,
					w * npa, 
					right + right_va,
					Color::Black);

			prior_left = left;
			prior_right = right;

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
	if(e.type == SDL_USEREVENT && e.user.code == UPDATE_CODE){
		request_draw = true;
	} else if(e.type == SDL_KEYDOWN){
		if(e.key.keysym.sym == SDLK_SPACE){
			if(Mix_PausedMusic()){
				paused = false;
				Mix_ResumeMusic();
			} else {
				paused = true;
				Mix_PauseMusic();
			}
		}

		if(e.key.keysym.sym == SDLK_UP) {
			volume += 2;
			Mix_VolumeMusic(volume);
		}
		if(e.key.keysym.sym == SDLK_DOWN) {
			volume -= 2;
			Mix_VolumeMusic(volume);
		}
	}
}

void Visualizer::ChangeSong(const char* fname){
	Mix_PauseMusic();
	this->fname = fname;
	song = Mix_LoadMUS(fname);

	if(!Mix_PlayingMusic()){
		Mix_PlayMusic(song,-1);
	}
	Mix_ResumeMusic();
}
