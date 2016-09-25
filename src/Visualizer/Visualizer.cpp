#include "Visualizer.h"

//The program flow for this is weird. There are 2 callbacks going at once, and they both depend on
//each other. The two callbacks do as follows
//1. Visualization Draw Request Callback (VDRC)
//   This is called after whenever SDL renews the data in the audio buffer. This gives the program
//   the oportunity to do any calculations it needs (FFT, effects, etc) and then send the draw calls
//   to the Graphics object. It then sets the request_screen_update flag, clears the audio draw request,
//   and returns.
//2. Screen Update Request Callback (SURC)
//   This is called 60 times a second to update the screen. It uses the 'diferred rendering' technique
//   used in the Graphics object to update the screen with the new visualizer lines created in the VDRC.
//   It then sets the request_audio_draw flag and clears its own.

Visualizer::Visualizer(Graphics *g, const char* fname) : g(g), fname(fname), song_name(""), artist(""),
    pause_button(Button(30,g->getHeight()-133,300,100,PLAY_BUTTON_CODE,{false,std::string("Pause"),Color::Green},"Play",Color::Red))
{

    auto gutter = 30;
    box_width = g->getWidth() - 2*gutter;
    box_height= g->getHeight()/2 + g->getHeight()/4;
    box_x = gutter;
    box_y = gutter;

    request_audio_draw = false;
    request_screen_update = false;

    if (!SDL_WasInit(SDL_INIT_AUDIO)) {
        SDL_Init(SDL_INIT_AUDIO);
    }

    auto open_success = Mix_OpenAudio(frequency,AUDIO_S16SYS,channels,chunksize);
    if(open_success <  0){
        printf("Error opening audio: %s\n",Mix_GetError());
    }

    //Load up the song!!
    if(std::string(fname) != "NO_SONG"){
        song = Mix_LoadMUS(fname);
    } else {
        printf("No file provided: Error %s\n",fname);
        paused = true;
    }

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

    //text_font_48 = TTF_OpenFont("assets/droid.ttf", 48);
    text_font_30 = TTF_OpenFont("assets/droid.ttf", 30);
    text_font_24 = TTF_OpenFont("assets/droid.ttf", 24);
    //	text_font_14 = TTF_OpenFont("assets/droid.ttf", 14);
    //	text_font_12 = TTF_OpenFont("assets/droid.ttf", 12);

}

Visualizer::~Visualizer(){
    Mix_CloseAudio();
    TTF_CloseFont(text_font_30);
    TTF_CloseFont(text_font_24);
    Mix_Quit();
}

uint32_t Visualizer::ScreenUpdateRequestCallback(uint32_t interval, void* param){
    //This method looks sorta wierd, but here's what it does.
    //It snags the pseudo-this out of the custom callback param for the timer
    auto _this = (Visualizer *) param;
    if(!_this->paused){
        _this->song_pos += anim_frame_delay;
    }

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


void do_effect(std::vector<std::pair<int8_t,int8_t>> &){
    
}

void Visualizer::AudioDrawRequestCallback(void *udata, uint8_t *dstream, int len){
    Visualizer * _this = (Visualizer *)udata;
    //Grab some stuff

    //If we have g, and there's a request to draw, draw the stuff
    if(_this->g != nullptr && _this->request_audio_draw){
        auto g = _this->g;

        int8_t prior_left = 0;
        int8_t prior_right = 0;

        //Stupid format of how it be
        std::vector<std::pair<int8_t,int8_t>> sample_data; //L,R
        for(int i = 5; i < len; i += 4){
            std::pair<int8_t,int8_t> p;
            p.first = *(dstream + i);
            p.second = *(dstream + i + 2);
            sample_data.push_back(p);
        }

        do_effect(sample_data);

        for(auto it = sample_data.begin() + 1; it < sample_data.end(); ++it){
            auto index = it - sample_data.begin();
            auto left = it->first;
            auto right = it->second;

            //Figure out how far along the buffer we are
            float ppa = static_cast<float>(index - 1)/static_cast<float>(sample_data.size());
            float npa = static_cast<float>(index)/static_cast<float>(sample_data.size());

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

        for(int i = 5; i < len; i += 4){
            *(dstream + i) = sample_data.begin()->first;
            *(dstream + i + 2) = sample_data.begin()->second;
            sample_data.erase(sample_data.begin());
        }
    }

    //The draw request has been satisfied, and now we signal that the new draw needs to be updated to the screen
    _this->request_audio_draw = false;
    _this->request_screen_update= true;
}


bool Visualizer::WaitingForWindowUpdate(){
    return request_screen_update;
}

//This is really sketchy. Found on SO, writes to c_str()... Maybe replace later. But it works for now.
std::string string_format(const std::string fmt_str, ...) {
    int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::string(formatted.get());
}

std::string Visualizer::FormattedSongTime(){
    int mins = static_cast<int>(floor(song_pos/60/1000));
    int secs = song_pos/1000 % 60;
    return string_format("%02d:%02d",mins,secs);
}

void Visualizer::RenderGui(void){
    //Show the song time
    g->Text(text_font_24,FormattedSongTime(),30,0);
    //Draw the box around the waveforms
    g->Rect(box_x,box_y,box_width,box_height,Color::Black);

    auto b = pause_button;
    g->FillRect(b.getX(), b.getY(), b.getWidth(), b.getHeight(), b.getColor());
    g->Text(text_font_30,b.getText(),b.getX() + 50,b.getY() + 30);

}

void Visualizer::UpdateWindow(){
    g->Clear();
    RenderGui();
    g->Update();
    request_screen_update = false;
}

void Visualizer::HandleEvent(SDL_Event e){
    if(e.type == SDL_USEREVENT && e.user.code == UPDATE_CODE){
        request_audio_draw = true;
    } else if(e.type == SDL_KEYDOWN){
        //Pause?
        if(e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_k){
            pause_button.toggleState();
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
    } else if(e.type == SDL_MOUSEBUTTONDOWN){
        auto press = e.button;
        if(pause_button.mouseInside(press.x,press.y)){
            pause_button.toggleState();
            paused = pause_button.isPressed();
            if(paused){
                Mix_PauseMusic();
            } else {
                Mix_ResumeMusic();
            }
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
    song_pos = 0;
    Mix_ResumeMusic();
}
