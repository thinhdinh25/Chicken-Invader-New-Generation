
#include "CommonFunc.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "ButtonObject.h"


#include <iostream>
#include <string>
BaseObject g_background;
int scrollingOffset = 0;


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
        //SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN);
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED );
        //SDL_RenderSetLogicalSize(g_screen, 400, 300);
        //SDL_RenderSetScale(g_screen, 2.5, 2.5);
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
    }
    return success;
}

bool LoadBackground(std::string path) {
    bool ret = g_background.LoadImg(path, g_screen);
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
int start = 0;
int main(int argc, char* argv[])
{
    ImpTimer fps_timer;
    ButtonObject start_button;
    ButtonObject setting_button;
    ButtonObject quit_button;
    if (InitData() == false) return -1;
    if (start == 0) {
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
    }
    else if (start == 1 ){ if (LoadBackground("img//main_bg.png") == false) return -1; }

    MainObject p_player;
    p_player.set_x_pos(SCREEN_WIDTH / 2);
    p_player.set_y_pos(SCREEN_HEIGHT * 4 / 5);
    p_player.LoadImg("img//character.png", g_screen);
    p_player.set_clips();
    ThreatsObject p_threat;
    bool is_quit = false;

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
                        if (LoadBackground("img//main_bg.png") == false) return -1;
                        start++;
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



            // Moving background down
            ++scrollingOffset;
            if (scrollingOffset >= g_background.GetRect().h)
            {
                scrollingOffset = 0;
            }

            // Render the first background image
            g_background.SetRect(0, scrollingOffset - g_background.GetRect().h);
            g_background.Render(g_screen, NULL);

            // Render the second background image, ensuring seamless transition
            g_background.SetRect(0, scrollingOffset);
            g_background.Render(g_screen, NULL);


            p_player.HandleBullet(g_screen);
            p_player.DoPlayer();
            p_player.Show(g_screen);

            p_threat.SpawnThreats(g_screen, 5);
            p_threat.HandleAnimation(g_screen);
            p_threat.HandleThreatBullet(g_screen);
            p_threat.HandleBrokenEgg(g_screen);

            // check collision between bullet and threats
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
                                p_player.RemoveBullet(i);
                                obj_threat->Free();
                                p_threat.RemoveThreat(j);
                            }
                        }
                    }
                }
            }


            //check collision between character and threat and threat object
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
                    if (bCol) {
                        std::cout << "game over";
                    }
                }
            }



            SDL_RenderPresent(g_screen);

            int real_imp_time = fps_timer.get_ticks();
            int time_one_frame = 1000 / FRAME_PER_SECOND; // ms

            if (real_imp_time < time_one_frame) {
                int delay_time = time_one_frame - real_imp_time;
                if (delay_time >= 0) SDL_Delay(delay_time);
            }
        }
    }
    close();
    return 0;
}