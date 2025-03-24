#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <windows.h>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

//Screen
const int FRAME_PER_SECOND = 60;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

//Color
const int COLOR_KEY_R = 0;
const int COLOR_KEY_G = 0;
const int COLOR_KEY_B = 0;

const int RENDER_DRAW_COLOR = 255;


#define TILE_SIZE 64
#define MAX_MAP_X 1280
#define MAX_MAP_Y 640



//typedef struct Map {
//	int start_x_;
//	int start_y_;
//
//	int max_x_;
//	int max_y_;
//
//	int tile[MAX_MAP_Y][MAX_MAP_X];
//	char* file_name_;
//
//};

typedef struct Input {
	int left_;
	int right_;
	int up_;
	int down_;
	int shoot_;

};
#endif