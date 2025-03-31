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

#include <sstream>
#include <algorithm>
#include <numeric>
#include <limits>

BaseObject g_background;
int scrollingOffset = 0;
TTF_Font* font_time_big;
TTF_Font* font_time_small;
TTF_Font* font_time_small2;
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
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
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
        font_time_small = TTF_OpenFont("font//Jersey25-Regular.ttf", 25);
        if (font_time_small == NULL) {
            success = false;
        }
        font_time_small2 = TTF_OpenFont("font//Jersey25-Regular.ttf", 20);
        if (font_time_small2 == NULL) {
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

struct Person {
    std::string name;
    int score;

    bool operator < (const Person& p) const {
        return score > p.score;
    }
};

std::vector < TextObject* > getTopScores() {

    const int topN = 10;
    const std::string filename = "score.txt";
    std::ifstream file(filename);
    std::vector < Person > people;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector < std::string > words;
        std::string word;

        while (iss >> word) {
            words.push_back(word);
        }

        if (words.size() < 2 || !std::all_of(words.back().begin(), words.back().end(), ::isdigit)) {
            continue;
        }

        try {
            int score = std::stoi(words.back());
            words.pop_back();

            std::string name = std::accumulate(words.begin(), words.end(), std::string(),
                [](const std::string& a,
                    const std::string& b) {
                        return a.empty() ? b : a + " " + b;
                });

            people.push_back({
              name,
              score
                });
        }
        catch (const std::exception&) {
            continue;
        }
    }

    std::sort(people.begin(), people.end());
    std::vector < TextObject* > ten;
    for (int i = 0; i < (people.size(), static_cast <size_t> (topN)); ++i) {
        TextObject* newten = new TextObject();
        newten->SetColor(TextObject::WHITE_TEXT);
        newten->SetText(people[i].name + " " + std::to_string(people[i].score));
        ten.push_back(newten);
    }
    return ten;
}

std::vector < ButtonObject* > heart_list;

long long score = 0;
int start = 0;
std::vector < BaseObject* > bg_vector;
std::string name_ = "";
int wave = 1;
void SetParameter(int start_, MainObject& p_player, ThreatsObject& p_threat, std::vector < TextObject* >& high_score_list) {
    name_ = "";
    start = start_;
    wave = 1;
    p_threat.RemoveAllBullet();
    p_threat.RemoveAllThreat();
    p_player.SpawnPlayer(g_screen);

    if (start_ == 0) {
        LoadBackground("img//start_bg.png");
    }
    else if (start_ == 1 || start_ == 1001 || start_ == 1000) {
        LoadBackground("img//main_bg.png");
    }
    else if (start_ == 999) {
        LoadBackground("img//game_over_bg.png");
    }

    g_background.SetRect(0, 0);
    if (start == 999) {
        SDL_StartTextInput();
        high_score_list = getTopScores();
    }
    else {
        SDL_StopTextInput();
    }

    if (start == 1 && heart_list.size() != 3) {
        heart_list.clear();
        score = 0;
        p_player.SpawnPlayer(g_screen);
        int store_x_pos = 10;
        for (int i = 0; i < 3; i++) {
            ButtonObject* new_heart = new ButtonObject();
            new_heart->LoadImg("img//heart.png", g_screen);
            new_heart->set_x_pos(store_x_pos + 5);

            new_heart->set_y_pos(10);
            heart_list.push_back(new_heart);
            store_x_pos += new_heart->get_width_frame_() + 10;
        }
    }
}

int volume = 100;
int difficulty = 1;
int music_type = 0;

int main(int argc, char* argv[]) {
    ImpTimer fps_timer;
    ButtonObject start_button;
    ButtonObject setting_button;
    ButtonObject quit_button;
    ButtonObject try_again_button;
    ButtonObject home_button;

    ButtonObject vol_holder;

    ButtonObject diff_holder;

    ButtonObject music_type_holder;

    ButtonObject return_button;

    ButtonObject continue_button;

    if (InitData() == false) return -1;

    // init some buttons
    if (start_button.LoadImg("img//Start_button.png", g_screen) == false) return -1;
    start_button.set_x_pos(SCREEN_WIDTH * 3 / 5);
    start_button.set_y_pos(SCREEN_HEIGHT * 2 / 5 + 5);

    if (setting_button.LoadImg("img//Option_button.png", g_screen) == false) return -1;
    setting_button.set_x_pos(start_button.get_x_pos() + start_button.get_width_frame_());
    setting_button.set_y_pos(start_button.get_y_pos());

    if (quit_button.LoadImg("img//Quit_button.png", g_screen) == false) return -1;
    quit_button.set_x_pos(setting_button.get_x_pos() + setting_button.get_width_frame_());
    quit_button.set_y_pos(setting_button.get_y_pos());

    std::vector < TextObject* > high_score_list = getTopScores();

    if (try_again_button.LoadImg("img//try_again_button.png", g_screen) == false) return -1;
    try_again_button.set_x_pos(SCREEN_WIDTH / 2 - try_again_button.get_width_frame_() / 2 - 10);
    try_again_button.set_y_pos(SCREEN_HEIGHT - try_again_button.get_height_frame_());
    Mix_PlayMusic(Mix_LoadMUS("Sound//start_music.wav"), -1);

    if (home_button.LoadImg("img//home_button.png", g_screen) == false) return -1;
    home_button.set_x_pos(try_again_button.get_x_pos() + try_again_button.get_width_frame_() - 1);
    home_button.set_y_pos(try_again_button.get_y_pos());

    if (home_button.LoadImg("img//home_button.png", g_screen) == false) return -1;
    home_button.set_x_pos(try_again_button.get_x_pos() + try_again_button.get_width_frame_() - 1);
    home_button.set_y_pos(try_again_button.get_y_pos());

    if (home_button.LoadImg("img//home_button.png", g_screen) == false) return -1;
    home_button.set_x_pos(try_again_button.get_x_pos() + try_again_button.get_width_frame_() - 1);
    home_button.set_y_pos(try_again_button.get_y_pos());

    if (vol_holder.LoadImg("img//sound_volume_button.png", g_screen) == false) return -1;
    vol_holder.set_x_pos(SCREEN_WIDTH / 2 - vol_holder.get_width_frame_() / 2 - 10);
    vol_holder.set_y_pos(SCREEN_HEIGHT / 2 - vol_holder.get_height_frame_());

    if (music_type_holder.LoadImg("img//music_type_button.png", g_screen) == false) return -1;
    music_type_holder.set_x_pos(vol_holder.get_x_pos());
    music_type_holder.set_y_pos(vol_holder.get_y_pos() + vol_holder.get_height_frame_() + 10);

    if (diff_holder.LoadImg("img//difficulty_button.png", g_screen) == false) return -1;
    diff_holder.set_x_pos(music_type_holder.get_x_pos());
    diff_holder.set_y_pos(music_type_holder.get_y_pos() + music_type_holder.get_height_frame_() + 10);

    if (continue_button.LoadImg("img//continue_button.png", g_screen) == false) return -1;
    continue_button.set_x_pos(music_type_holder.get_x_pos());
    continue_button.set_y_pos(music_type_holder.get_y_pos() + music_type_holder.get_height_frame_() + 10);

    if (return_button.LoadImg("img//return_main.png", g_screen) == false) return -1;
    return_button.set_x_pos(diff_holder.get_x_pos());
    return_button.set_y_pos(diff_holder.get_y_pos() + diff_holder.get_height_frame_() + 10);

    int store_x_pos = 10;
    for (int i = 0; i < 3; i++) {
        ButtonObject* new_heart = new ButtonObject();
        new_heart->LoadImg("img//heart.png", g_screen);
        new_heart->set_x_pos(store_x_pos + 5);

        new_heart->set_y_pos(10);
        heart_list.push_back(new_heart);
        store_x_pos += new_heart->get_width_frame_() + 10;
    }

    MainObject p_player;
    p_player.SpawnPlayer(g_screen);
    ThreatsObject p_threat;
    bool is_quit = false;

    TextObject score_game;
    score_game.SetColor(TextObject::WHITE_TEXT);

    TextObject current_score;
    current_score.SetColor(TextObject::WHITE_TEXT);

    TextObject high_score_banner;
    high_score_banner.SetColor(TextObject::WHITE_TEXT);
    high_score_banner.SetText("************HIGH SCORE*************");

    TextObject vol_text;
    vol_text.SetColor(TextObject::WHITE_TEXT);
    vol_text.SetText(std::to_string(volume));

    TextObject music_type_text;
    music_type_text.SetColor(TextObject::WHITE_TEXT);
    music_type_text.SetText(std::to_string(music_type));

    TextObject difficulty_text;
    difficulty_text.SetColor(TextObject::WHITE_TEXT);
    difficulty_text.SetText(std::to_string(difficulty));

    static std::mt19937 gen(std::random_device{}());

    SetParameter(0, p_player, p_threat, high_score_list);

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
                        start_button.state_change(g_screen, true, "img//Start_button_hover.png", "img//Start_button.png");
                    }
                    else {
                        start_button.state_change(g_screen, false, "img//Start_button_hover.png", "img//Start_button.png");
                    }

                    if (setting_button.is_hovered_(g_event)) {
                        setting_button.state_change(g_screen, true, "img//Option_button_hover.png", "img//Option_button.png");
                    }
                    else {
                        setting_button.state_change(g_screen, false, "img//Option_button_hover.png", "img//Option_button.png");
                    }
                    if (quit_button.is_hovered_(g_event)) {
                        quit_button.state_change(g_screen, true, "img//Quit_button_hover.png", "img//Quit_button.png");
                    }
                    else {
                        quit_button.state_change(g_screen, false, "img//Quit_button_hover.png", "img//Quit_button.png");
                    }
                }
                else if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                    if (start_button.is_clicked_(g_event)) {
                        SetParameter(1, p_player, p_threat, high_score_list);
                    }
                    if (setting_button.is_clicked_(g_event)) {
                        SetParameter(1000, p_player, p_threat, high_score_list);
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
                else if (g_event.type == SDL_KEYDOWN) {
                    if (g_event.key.keysym.sym == SDLK_ESCAPE) {
                        g_background.SetRect(0, 0);
                        start = 1001;

                    }
                }
                p_player.HandleInputAction(g_event, g_screen);
            }

            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            SDL_RenderClear(g_screen);

            ++scrollingOffset;
            if (scrollingOffset >= g_background.GetRect().h) {
                scrollingOffset = 0;
            }
            if (start != 1001) {
                g_background.SetRect(0, scrollingOffset - g_background.GetRect().h);
                g_background.Render(g_screen, NULL);

                g_background.SetRect(0, scrollingOffset);
                g_background.Render(g_screen, NULL);
            }
            p_player.HandleBullet(g_screen);
            p_player.DoPlayer(g_screen);
            p_player.Show(g_screen);

            if (wave == 1) {
                p_threat.SpawnThreats(g_screen, 5);
            }
            else if (wave == 2) {
                p_threat.SpawnThreatsTriangle(g_screen, 28);
            }
            else if (wave == 3) {
                p_threat.SpawnThreatsCircle(g_screen, 15);
            }
            else if (wave == 4) {
                p_threat.SpawnBoss(g_screen);
            }
            p_threat.HandleAnimation(g_screen);
            p_threat.HandleThreatBullet(g_screen);
            p_threat.HandleBrokenEgg(g_screen);

            for (auto i : heart_list) {
                i->Show(g_screen);
            }

            // check collision between character bullet and threats
            std::vector < BulletObject* > bullet_arr = p_player.get_bullet_list();
            for (unsigned int i = 0; i < bullet_arr.size(); i++) {
                BulletObject* p_bullet = bullet_arr.at(i);
                if (p_bullet != NULL) {
                    std::vector < ThreatsObject* > threat_arr = p_threat.get_threat_list();
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
                                if (wave != 4) {
                                    score += std::uniform_int_distribution < int >(5000, 15000)(gen);
                                    Mix_PlayChannel(-1, Mix_LoadWAV("Sound//chicken_killed.wav"), 0);
                                    p_player.RemoveBullet(i);
                                    obj_threat->Free();
                                    p_threat.RemoveThreat(j);
                                }
                                else if (wave == 4) {
                                    if (obj_threat->GetHitCount() < 30) {
                                        if (SDL_GetTicks() - obj_threat->GetHitTime() > 300) {
                                            obj_threat->UpdateHitTime();
                                            score += std::uniform_int_distribution < int >(5000, 15000)(gen);
                                            Mix_PlayChannel(-1, Mix_LoadWAV("Sound//chicken_killed.wav"), 0);
                                            obj_threat->UpdateHitCount(obj_threat->GetHitCount() + 1);

                                        }
                                        p_player.RemoveBullet(i);
                                    }
                                    else {
                                        score += std::uniform_int_distribution < int >(5000, 15000)(gen);
                                        Mix_PlayChannel(-1, Mix_LoadWAV("Sound//chicken_killed.wav"), 0);
                                        p_player.RemoveBullet(i);
                                        obj_threat->Free();
                                        p_threat.RemoveThreat(j);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //check collision between character and threat object
            std::vector < ThreatsObject* > threat_arr = p_threat.get_threat_list();
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
                        heart_list.erase(heart_list.end() - 1);
                        if (heart_) {
                            delete heart_;
                            heart_ = NULL;
                        }
                    }
                }
            }

            //check collision between character and threat bullet
            std::vector < ThreatsObject* > threat_bullet_arr = p_threat.get_threat_bullet_list();
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
                        if (heart_list.size() > 0) {
                            ButtonObject* heart_ = heart_list.at(heart_list.size() - 1);
                            heart_list.erase(heart_list.end() - 1);
                            if (heart_) {
                                delete heart_;
                                heart_ = NULL;
                            }
                        }
                    }
                }
            }

            // check remaining threats
            if (threat_arr.size() == 0) wave++;
            if (wave > 4) {
                p_player.SpawnPlayer(g_screen);
                for (int i = static_cast <int> (threat_arr.size()) - 1; i >= 0; i--) {
                    p_threat.RemoveThreat(i);
                }
                for (int i = static_cast <int> (threat_bullet_arr.size()) - 1; i >= 0; i--) {
                    p_threat.RemoveBullet(i);
                }
                SetParameter(999, p_player, p_threat, high_score_list);
                wave = 1;
            }

            //Check Game Over
            if (heart_list.size() == 0) {
                p_player.SpawnPlayer(g_screen);
                for (int i = static_cast <int> (threat_arr.size()) - 1; i >= 0; i--) {
                    p_threat.RemoveThreat(i);
                }
                for (int i = static_cast <int> (threat_bullet_arr.size()) - 1; i >= 0; i--) {
                    p_threat.RemoveBullet(i);
                }
                SetParameter(999, p_player, p_threat, high_score_list);
                wave = 1;

            }

            //Show Game Score
            score_game.SetText("Score: " + std::to_string(score));
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
                        SetParameter(1, p_player, p_threat, high_score_list);
                    }
                    if (home_button.is_clicked_(g_event)) {
                        SetParameter(0, p_player, p_threat, high_score_list);
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
                            SDLCommonFunc::SaveToFile(name_, score);
                            high_score_list = getTopScores();
                            name_ = "SAVED";

                        }
                    }
                }
            }

            score_game.SetText("Your Score: " + std::to_string(score));
            score_game.LoadFromRenderText(font_time_small, g_screen);
            score_game.RenderText(g_screen, SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 50);

            current_score.SetText("Enter Your Name: " + name_);
            current_score.LoadFromRenderText(font_time_small, g_screen);
            current_score.RenderText(g_screen, SCREEN_WIDTH * 2 / 5, SCREEN_HEIGHT * 3 / 5);

            high_score_banner.LoadFromRenderText(font_time_small, g_screen);
            high_score_banner.RenderText(g_screen, SCREEN_WIDTH - 320, 20);

            int h_val = 50;
            for (auto high_score : high_score_list) {
                high_score->LoadFromRenderText(font_time_small, g_screen);
                high_score->RenderText(g_screen, SCREEN_WIDTH - 320, h_val);
                h_val += 30;
            }

            SDL_RenderPresent(g_screen);
        }
        else if (start == 1000) {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            SDL_RenderClear(g_screen);
            while (SDL_PollEvent(&g_event) != 0) {
                if (g_event.type == SDL_QUIT) {
                    is_quit = true;
                }
                else if (g_event.type == SDL_MOUSEMOTION) {
                    if (vol_holder.is_hovered_(g_event)) {
                        vol_holder.LoadImg("img//sound_volume_button_hover.png", g_screen);
                    }
                    else {
                        vol_holder.LoadImg("img//sound_volume_button.png", g_screen);
                    }

                    if (diff_holder.is_hovered_(g_event)) {
                        diff_holder.LoadImg("img//difficulty_button_hover.png", g_screen);
                    }
                    else {
                        diff_holder.LoadImg("img//difficulty_button.png", g_screen);
                    }
                    if (music_type_holder.is_hovered_(g_event)) {
                        music_type_holder.LoadImg("img//music_type_button_hover.png", g_screen);
                    }
                    else {
                        music_type_holder.LoadImg("img//music_type_button.png", g_screen);
                    }

                    if (return_button.is_hovered_(g_event)) {
                        return_button.LoadImg("img//return_main_hover.png", g_screen);
                    }
                    else {
                        return_button.LoadImg("img//return_main.png", g_screen);
                    }
                }
                else if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                    if (vol_holder.is_clicked_(g_event)) {
                        volume += 25;
                        if (volume > 100) volume = 0;
                        Mix_VolumeMusic(volume);
                        if (music_type == 0)
                            Mix_Volume(-1, volume);
                        vol_text.SetText(std::to_string(volume));
                    }
                    if (diff_holder.is_clicked_(g_event)) {
                        difficulty += 1;
                        if (difficulty > 3) difficulty = 1;
                        p_threat.UpdateGravitySpeed(2 * difficulty);

                        difficulty_text.SetText(std::to_string(difficulty));

                    }
                    if (music_type_holder.is_clicked_(g_event)) {
                        music_type += 1;
                        if (music_type > 1) music_type = 0;
                        Mix_HaltMusic();
                        if (music_type == 0) {
                            Mix_PlayMusic(Mix_LoadMUS("Sound//start_music.wav"), -1);
                            Mix_Volume(-1, volume);
                        }
                        else {
                            Mix_PlayMusic(Mix_LoadMUS("Sound//remix.ogg"), -1);
                            Mix_Volume(-1, 0);
                        }
                        music_type_text.SetText(std::to_string(music_type));
                    }
                    if (return_button.is_clicked_(g_event)) {
                        SetParameter(0, p_player, p_threat, high_score_list);
                    }

                }
            }

            g_background.Render(g_screen, NULL);

            vol_holder.Show(g_screen);
            diff_holder.Show(g_screen);
            music_type_holder.Show(g_screen);
            return_button.Show(g_screen);

            vol_text.LoadFromRenderText(font_time_small2, g_screen);
            vol_text.RenderText(g_screen, vol_holder.get_x_pos() + vol_holder.get_width_frame_() - 50, vol_holder.get_y_pos() + vol_holder.get_height_frame_() / 4);

            music_type_text.LoadFromRenderText(font_time_small2, g_screen);
            music_type_text.RenderText(g_screen, music_type_holder.get_x_pos() + music_type_holder.get_width_frame_() - 50, music_type_holder.get_y_pos() + music_type_holder.get_height_frame_() / 4);

            difficulty_text.LoadFromRenderText(font_time_small2, g_screen);
            difficulty_text.RenderText(g_screen, diff_holder.get_x_pos() + diff_holder.get_width_frame_() - 50, diff_holder.get_y_pos() + diff_holder.get_height_frame_() / 4);

            SDL_RenderPresent(g_screen);
        }
        else if (start == 1001) {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            SDL_RenderClear(g_screen);
            while (SDL_PollEvent(&g_event) != 0) {
                if (g_event.type == SDL_QUIT) {
                    is_quit = true;
                }
                else if (g_event.type == SDL_MOUSEMOTION) {
                    if (vol_holder.is_hovered_(g_event)) {
                        vol_holder.LoadImg("img//sound_volume_button_hover.png", g_screen);
                    }
                    else {
                        vol_holder.LoadImg("img//sound_volume_button.png", g_screen);
                    }

                    if (continue_button.is_hovered_(g_event)) {
                        continue_button.LoadImg("img//continue_button_hover.png", g_screen);
                    }
                    else {
                        continue_button.LoadImg("img//continue_button.png", g_screen);
                    }
                    if (music_type_holder.is_hovered_(g_event)) {
                        music_type_holder.LoadImg("img//music_type_button_hover.png", g_screen);
                    }
                    else {
                        music_type_holder.LoadImg("img//music_type_button.png", g_screen);
                    }

                    if (return_button.is_hovered_(g_event)) {
                        return_button.LoadImg("img//return_main_hover.png", g_screen);
                    }
                    else {
                        return_button.LoadImg("img//return_main.png", g_screen);
                    }
                }
                else if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                    if (vol_holder.is_clicked_(g_event)) {
                        volume += 25;
                        if (volume > 100) volume = 0;
                        Mix_VolumeMusic(volume);
                        if (music_type == 0)
                            Mix_Volume(-1, volume);
                        vol_text.SetText(std::to_string(volume));
                    }
                    if (diff_holder.is_clicked_(g_event)) {
                        g_background.SetRect(0, 0);
                        start = 1;

                    }
                    if (music_type_holder.is_clicked_(g_event)) {
                        music_type += 1;
                        if (music_type > 1) music_type = 0;
                        Mix_HaltMusic();
                        if (music_type == 0) {
                            Mix_PlayMusic(Mix_LoadMUS("Sound//start_music.wav"), -1);
                            Mix_Volume(-1, volume);
                        }
                        else {
                            Mix_PlayMusic(Mix_LoadMUS("Sound//remix.ogg"), -1);
                            Mix_Volume(-1, 0);
                        }
                        music_type_text.SetText(std::to_string(music_type));
                    }
                    if (return_button.is_clicked_(g_event)) {
                        SetParameter(0, p_player, p_threat, high_score_list);

                    }

                }
            }

            g_background.Render(g_screen, NULL);

            vol_holder.Show(g_screen);
            continue_button.Show(g_screen);
            music_type_holder.Show(g_screen);
            return_button.Show(g_screen);

            vol_text.LoadFromRenderText(font_time_small2, g_screen);
            vol_text.RenderText(g_screen, vol_holder.get_x_pos() + vol_holder.get_width_frame_() - 50, vol_holder.get_y_pos() + vol_holder.get_height_frame_() / 4);

            music_type_text.LoadFromRenderText(font_time_small2, g_screen);
            music_type_text.RenderText(g_screen, music_type_holder.get_x_pos() + music_type_holder.get_width_frame_() - 50, music_type_holder.get_y_pos() + music_type_holder.get_height_frame_() / 4);

            SDL_RenderPresent(g_screen);
        }
    }
    close();
    return 0;
}