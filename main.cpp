
#include "CommonFunc.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "ButtonObject.h"
#include "TextObject.h"

#include <iostream>
#include <string>
#include <fstream>
#include <random>

BaseObject g_background;
int scrollingOffset = 0;
TTF_Font* font_time_big;
TTF_Font* font_time_small;
bool InitData() {
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow("Chicken Invader", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == NULL) {
        success = false;
    }
    else {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED );
        if (g_screen == NULL) { 
            success = false;
        }
        else {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags)) success = false;
            int img2Flags = IMG_INIT_JPG;
            if (!(IMG_Init(img2Flags) && img2Flags)) success = false;
        }
        if (TTF_Init() == -1) {
            success = false;
        }

        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            success = false;
        }
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            success = false;
        }
        font_time_big = TTF_OpenFont("font//Jersey25-Regular.ttf", 40);
        if (font_time_big == NULL) {
            success = false;
        }
        font_time_small = TTF_OpenFont("font//arial.ttf", 20);
        if (font_time_small == NULL) {
            success = false;
        }
    }
    return success;
}

bool LoadBackground(std::string path) {
    bool ret = g_background.LoadImg1(path, g_screen);
    if (ret == false) {
        printf("Failed to load background image! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

void close() {
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();

}
int start = 999;
std::string name_ = "";
void SetParameter(int start_, const std::string &img_Path, MainObject &p_player)
{
    name_ = "";
    start = start_;
    p_player.SetSpawnTime(SDL_GetTicks());
    LoadBackground(img_Path);
    if (start == 999) {
        SDL_StartTextInput(); 
    }
    else {
        SDL_StopTextInput();
    }
}
    



int main(int argc, char* argv[])
{
    ImpTimer fps_timer;
    ButtonObject start_button;
    ButtonObject setting_button;
    ButtonObject quit_button;
    ButtonObject try_again_button;
    ButtonObject home_button;

    if (InitData() == false) return -1;
    // init some buttons
    if (LoadBackground("img//start_bg.png") == false) return -1; 
    if (start_button.LoadImg("img//Start_button.png", g_screen) == false) return -1;
    start_button.set_x_pos(SCREEN_WIDTH *3/5);
    start_button.set_y_pos(SCREEN_HEIGHT *2/5+5);

    if (setting_button.LoadImg("img//Option_button.png", g_screen) == false) return -1;
    setting_button.set_x_pos(start_button.get_x_pos() + start_button.get_width_frame_());
    setting_button.set_y_pos(start_button.get_y_pos());

    if (quit_button.LoadImg("img//Quit_button.png", g_screen) == false) return -1;
    quit_button.set_x_pos(setting_button.get_x_pos() + setting_button.get_width_frame_());
    quit_button.set_y_pos(setting_button.get_y_pos());


    if (LoadBackground("img//game_over_bg.png") == false) return -1;
    if (try_again_button.LoadImg("img//try_again_button.png", g_screen) == false) return -1;
    try_again_button.set_x_pos(SCREEN_WIDTH/2 - try_again_button.get_width_frame_()/2 - 10);
    try_again_button.set_y_pos(SCREEN_HEIGHT - try_again_button.get_height_frame_());
    Mix_PlayMusic(Mix_LoadMUS("Sound//start_music.wav"), -1);

    if (home_button.LoadImg("img//home_button.png", g_screen) == false) return -1;
    home_button.set_x_pos(try_again_button.get_x_pos() + try_again_button.get_width_frame_()-1);
    home_button.set_y_pos(try_again_button.get_y_pos());

        //Mix_VolumeMusic(0);
        //Mix_Volume(-1, 0);
    

    MainObject p_player;
    p_player.SpawnPlayer(g_screen);
    ThreatsObject p_threat;
    bool is_quit = false;

    TextObject score_game;
    score_game.SetColor(TextObject::WHITE_TEXT);

    
    TextObject high_score;
    score_game.SetColor(TextObject::WHITE_TEXT);

    static std::mt19937 gen(std::random_device{}());
    long long score = 0;

    std::vector<ButtonObject*> heart_list;
    int store_x_pos = 10;
    for (int i = 0; i < 3; i++) {
        ButtonObject* new_heart = new ButtonObject();
        new_heart->LoadImg("img//heart.png", g_screen);
        new_heart->set_x_pos(store_x_pos + 5);

        new_heart->set_y_pos(10);
        heart_list.push_back(new_heart);
        store_x_pos += new_heart->get_width_frame_() + 10;
    }


    while (!is_quit) {
        if (start == 0) {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            SDL_RenderClear(g_screen);
            g_background.Render(g_screen, NULL);
            start_button.Show(g_screen);
            setting_button.Show(g_screen);
            quit_button.Show(g_screen);
            while (SDL_PollEvent(&g_event) != 0) {
                if (g_event.type == SDL_QUIT) {
                    is_quit = true;
                }
                else if (g_event.type == SDL_MOUSEMOTION) {
                    if (start_button.is_hovered_(g_event)) {
                        start_button.LoadImg("img//Start_button_hover.png",g_screen);
                    }
                    else {
                        start_button.LoadImg("img//Start_button.png", g_screen);
                    }

                    if (setting_button.is_hovered_(g_event)) {
                        setting_button.LoadImg("img//Option_button_hover.png", g_screen);
                    }
                    else {
                        setting_button.LoadImg("img//Option_button.png", g_screen);
                    }
                    if (quit_button.is_hovered_(g_event)) {
                        quit_button.LoadImg("img//Quit_button_hover.png", g_screen);
                    }
                    else {
                        quit_button.LoadImg("img//Quit_button.png", g_screen);
                    }
                }
                else if (g_event.type == SDL_MOUSEBUTTONDOWN){
                    if (start_button.is_clicked_(g_event)) {     
                        SetParameter(1, "img//main_bg.png", p_player);
                    }
                    if (setting_button.is_clicked_(g_event)) {
                        setting_button.LoadImg("img//Option_button_hover.png", g_screen);
                    
                    }
                    if (quit_button.is_clicked_(g_event)) {
                        is_quit = true;
                    }
                }
                
                
            }

            SDL_RenderPresent(g_screen);
        }

        else if (start == 1) {
            fps_timer.start();

            while (SDL_PollEvent(&g_event) != 0) {
                if (g_event.type == SDL_QUIT) {
                    is_quit = true;
                }
                p_player.HandleInputAction(g_event, g_screen);
            }

            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            SDL_RenderClear(g_screen);



            ++scrollingOffset;
            if (scrollingOffset >= g_background.GetRect().h)
            {
                scrollingOffset = 0;
            }

            g_background.SetRect(0, scrollingOffset - g_background.GetRect().h);
            g_background.Render(g_screen, NULL);

            g_background.SetRect(0, scrollingOffset);
            g_background.Render(g_screen, NULL);


            p_player.HandleBullet(g_screen);
            p_player.DoPlayer(g_screen);
            p_player.Show(g_screen);

            p_threat.SpawnThreats(g_screen, 5);
            p_threat.HandleAnimation(g_screen);
            p_threat.HandleThreatBullet(g_screen);
            p_threat.HandleBrokenEgg(g_screen);

            for (auto i : heart_list) {
                i->Show(g_screen);
            }



            // check collision between character bullet and threats
            std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
            for (unsigned int i = 0; i < bullet_arr.size(); i++) {
                BulletObject* p_bullet = bullet_arr.at(i);
                if (p_bullet != NULL) {
                    std::vector<ThreatsObject*> threat_arr = p_threat.get_threat_list();
                    for (unsigned int j = 0; j < threat_arr.size(); j++) {
                        ThreatsObject* obj_threat = threat_arr.at(j);
                        if (obj_threat != NULL) {
                            SDL_Rect tRect;
                            tRect.x = obj_threat->GetRect().x;
                            tRect.y = obj_threat->GetRect().y;
                            tRect.w = obj_threat->get_width_frame() - 40;
                            tRect.h = obj_threat->get_height_frame();

                            SDL_Rect bRect;
                            bRect.x = p_bullet->GetRect().x;
                            bRect.y = p_bullet->GetRect().y;
                            bRect.w = p_bullet->GetRect().w - 20;
                            bRect.h = p_bullet->GetRect().h;
                            bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);
                            if (bCol) {
                                score += std::uniform_int_distribution<int>(5000, 15000)(gen);
                                Mix_PlayChannel(-1, Mix_LoadWAV("Sound//chicken_killed.wav"), 0);
                                p_player.RemoveBullet(i);
                                obj_threat->Free();
                                p_threat.RemoveThreat(j);
                            }
                        }
                    }
                }
            }


            //check collision between character and threat object
            std::vector<ThreatsObject*> threat_arr = p_threat.get_threat_list();
            for (unsigned int i = 0; i < threat_arr.size(); i++) {
                ThreatsObject* obj_threat = threat_arr.at(i);
                if (obj_threat != NULL) {
                    SDL_Rect tRect;
                    tRect.x = obj_threat->GetRect().x;
                    tRect.y = obj_threat->GetRect().y;
                    tRect.w = obj_threat->get_width_frame();
                    tRect.h = obj_threat->get_height_frame();

                    SDL_Rect bRect;
                    bRect.x = p_player.GetRect().x;
                    bRect.y = p_player.GetRect().y;
                    bRect.w = p_player.get_width_frame();
                    bRect.h = p_player.get_height_frame();
                    bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);
                    if (bCol && SDL_GetTicks() - p_player.GetSpawnTime() >= 3000 && p_player.get_is_shown() == 1) {
                        Mix_PlayChannel(-1, Mix_LoadWAV("Sound//player_destroy.wav"), 0);
                        p_player.ReSpawn(g_screen);
                        ButtonObject* heart_ = heart_list.at(heart_list.size() - 1);
                        heart_list.erase(heart_list.end()-1);
                        if (heart_) {
                            delete heart_;
                            heart_ = NULL;
                        }
                    }
                }
            }

            //check collision between character and threat bullet
            std::vector<ThreatsObject*> threat_bullet_arr = p_threat.get_threat_bullet_list();
            for (unsigned int i = 0; i < threat_bullet_arr.size(); i++) {
                ThreatsObject* obj_threat = threat_bullet_arr.at(i);
                if (obj_threat != NULL) {
                    SDL_Rect tRect;
                    tRect.x = obj_threat->GetRect().x;
                    tRect.y = obj_threat->GetRect().y;
                    tRect.w = obj_threat->get_width_frame();
                    tRect.h = obj_threat->get_height_frame();

                    SDL_Rect bRect;
                    bRect.x = p_player.GetRect().x;
                    bRect.y = p_player.GetRect().y;
                    bRect.w = p_player.get_width_frame();
                    bRect.h = p_player.get_height_frame();
                    bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);
                    if (bCol && SDL_GetTicks() - p_player.GetSpawnTime() >= 3000 && p_player.get_is_shown() == 1) {
                        Mix_PlayChannel(-1, Mix_LoadWAV("Sound//player_destroy.wav"), 0);
                        p_player.ReSpawn(g_screen);
                        if (heart_list.size() > 0){
                        ButtonObject* heart_ = heart_list.at(heart_list.size()-1);
                        heart_list.erase(heart_list.end()-1);
                        if (heart_) {
                            delete heart_;
                            heart_ = NULL;
                        }}
                    }
                }
            }


            //Check Game Over
            if (heart_list.size() == 0) {
                SetParameter(999, "img//game_over_bg.png", p_player);
            }


            //Show Game Score
            std::string str_time = "Score";
            score_game.SetText(str_time);
            score_game.LoadFromRenderText(font_time_big, g_screen);
            score_game.RenderText(g_screen, 150, 5);

            SDL_RenderPresent(g_screen);

            int real_imp_time = fps_timer.get_ticks();
            int time_one_frame = 1000 / FRAME_PER_SECOND; // ms

            if (real_imp_time < time_one_frame) {
                int delay_time = time_one_frame - real_imp_time;
                if (delay_time >= 0) SDL_Delay(delay_time);
            }
        }
        else if (start == 999) {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            SDL_RenderClear(g_screen);
            g_background.Render(g_screen, NULL);
            try_again_button.Show(g_screen);
            home_button.Show(g_screen);

            
            while (SDL_PollEvent(&g_event) != 0) {
                if (g_event.type == SDL_QUIT) {
                    is_quit = true;
                }
                else if (g_event.type == SDL_MOUSEMOTION) {
                    if (try_again_button.is_hovered_(g_event)) {
                        try_again_button.LoadImg("img//try_again_button_hover.png", g_screen);
                    }
                    else {
                        try_again_button.LoadImg("img//try_again_button.png", g_screen);
                    }

                    if (home_button.is_hovered_(g_event)) {
                        home_button.LoadImg("img//home_button_hover.png", g_screen);
                    }
                    else {
                        home_button.LoadImg("img//home_button.png", g_screen);
                    }

                }
                else if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                    if (try_again_button.is_clicked_(g_event)) {
                        if (LoadBackground("img//main_bg.png") == false) return -1;
                        start = 1;
                        p_player.SetSpawnTime(SDL_GetTicks());

                    }
                    if (home_button.is_clicked_(g_event)) {
                        if (LoadBackground("img//start_bg.png") == false) return -1;
                        start = 0;
                    }
                }
                else if (g_event.type == SDL_TEXTINPUT) {
                    if (name_ != "SAVED")
                        name_ += g_event.text.text;
                }
                else if (g_event.type == SDL_KEYDOWN) {
                    if (g_event.key.keysym.sym == SDLK_BACKSPACE && name_.length() > 0) {
                        if (name_ != "SAVED")
                            name_.pop_back();
                    }
                    else if (g_event.key.keysym.sym == SDLK_RETURN) {
                        if (name_ != "") {
                            std::cout << "User entered: " << name_ << std::endl;
                            SDLCommonFunc::SaveToFile(name_,score);
                            name_ = "SAVED";
                        }
                    }
                }
            }

            score_game.SetText("Your Score: " + std::to_string(score));
            score_game.LoadFromRenderText(font_time_small, g_screen);
            score_game.RenderText(g_screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3 / 5);


            high_score.SetText("Enter Your Name: " +  name_);
            high_score.LoadFromRenderText(font_time_small, g_screen);
            high_score.RenderText(g_screen, SCREEN_WIDTH*2/5 , SCREEN_HEIGHT*3/5+50);


           
            SDL_RenderPresent(g_screen);
        }
    }
    close();
    return 0;
}