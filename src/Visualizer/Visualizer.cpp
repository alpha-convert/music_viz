#include "Visualizer.h"
Visualizer::Visualizer(Graphics *g, const char* fname){
	this->g = g;
	this->fname = fname;
	this->songname = fname;

	auto gutter = 30;
	box_width = g->width - 2*gutter;
	box_height= g->height/2 + g->height/4;
	box_x = gutter;
	box_y = gutter;

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



	if(!TTF_WasInit() && TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
	}

	text_font_30 = TTF_OpenFont("assets/droid.ttf", 30);
	text_font_24 = TTF_OpenFont("assets/droid.ttf", 24);
	text_font_14 = TTF_OpenFont("assets/droid.ttf", 14);
	text_font_12 = TTF_OpenFont("assets/droid.ttf", 12);

}
Visualizer::~Visualizer(){
	Mix_CloseAudio();
	TTF_CloseFont(text_font_30);
	TTF_CloseFont(text_font_24);
	TTF_CloseFont(text_font_14);
	TTF_CloseFont(text_font_12);
	Mix_Quit();
}

uint32_t Visualizer::ScreenUpdateRequestCallback(uint32_t interval, void* param){
	//This method looks sorta wierd, but here's what it does.
	//It snags the pseudo-this out of the custom callback param for future use
	auto _this = (Visualizer *) param;
	USE(_this);

	//Creates an event
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = UPDATE_CODE;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	//Pushes that event onto the event queue
	//The event loop in main() will snag this event, and signal to the window that it's ready
	//for the next round.                                                         
	SDL_PushEvent(&event);
	return(interval);
}
void Visualizer::AudioDrawRequestCallback(void *udata, uint8_t *dstream, int len){
	Visualizer * _this = (Visualizer *)udata;
	//Grab some stuff

	//If we have g, and there's a request to draw, draw the stuff
	if(_this->g != nullptr && _this->request_draw){
		auto g = _this->g;

		int8_t prior_left = 0;
		int16_t prior_right = 0;

		//Starts at 5,
		//Stride 4, begins at 1, need to go one ahead for (index - 4) to be > 0
		for(int index = 5; index < len; index = index + 4){
			//Grab the value out of the left and right channels;
			int8_t left = *(dstream + index);
			int8_t right = *(dstream + index + 2);

			//Figure out how far along the buffer we are
			float ppa = static_cast<float>(index - 4)/static_cast<float>(len);
			float npa = static_cast<float>(index)/static_cast<float>(len);

			//Left and right vertial offsets;
			auto left_va = _this->box_height/4;
			auto right_va = 3*_this->box_height/4;

			//Draw the line pieces.
			auto l_x0 = _this->box_x + _this->box_width * ppa;
			auto l_y0 = _this->box_y + prior_left + left_va;
			auto l_x1 = _this->box_x + _this->box_width * npa;
			auto l_y1 = _this->box_y + left + left_va;

			g->Line(l_x0,l_y0,l_x1,l_y1,Color::Black);

			auto r_x0 = _this->box_x + _this->box_width * ppa;
			auto r_y0 = _this->box_y + prior_right+ right_va;
			auto r_x1 = _this->box_x + _this->box_width * npa;
			auto r_y1 = _this->box_y + right + right_va;

			g->Line(r_x0,r_y0,r_x1,r_y1,Color::Black);

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

void Visualizer::Text(TTF_Font *font, const char *strbuf, unsigned int x, unsigned int y, const Color &c = Color::Black){
	SDL_Color s_c;
	s_c.r = c.r*255;
	s_c.g = c.g*255;
	s_c.b = c.b*255;
	s_c.a = c.a*255;
	auto text_surface = TTF_RenderText_Blended(font, strbuf, s_c);
	auto text_texture = SDL_CreateTextureFromSurface(g->getRenderer(), text_surface); 

	int text_width;
	int text_height;
	auto err = TTF_SizeText(font, strbuf, &text_width, &text_height);
	assert(err == 0);

	SDL_Rect container; 
	container.x = x;  
	container.y = y; 
	container.w = text_width;
	container.h = text_height;

	SDL_RenderCopy(g->getRenderer(), text_texture, NULL, &container);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);

}


void Visualizer::RenderGui(void){
	g->Rect(box_x,box_y,box_width,box_height,Color::Black);
}

void Visualizer::UpdateWindow(){
	g->Clear();
	RenderGui();
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
