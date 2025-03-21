
#include "CommonFunc.h"
#include "BaseObject.h"
#include "MainObject.h"

BaseObject g_background;



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
    }
    return success;
}

bool LoadBackground() {
    bool ret = g_background.LoadImg("img//620478.jpg", g_screen);
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

int main(int argc, char* argv[])
{
    if (InitData() == false) return -1;
    if (LoadBackground() == false) return -1;

    MainObject p_player;
    p_player.LoadImg("img//character.png",g_screen);
    p_player.set_clips();

    bool is_quit = false;

    while (!is_quit) {
        while (SDL_PollEvent(&g_event) != 0) {
            if (g_event.type == SDL_QUIT) {
                is_quit = true;
            }

            p_player.HandleInputAction(g_event, g_screen);
        }
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);

        p_player.Show(g_screen);

        SDL_RenderPresent(g_screen);
    }
    close();
    return 0;
}