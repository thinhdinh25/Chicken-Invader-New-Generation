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
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
const int SCREEN_BPP = 32;
const float SCALE_NUMBER = 1;

//Color
const int COLOR_KEY_R = 0;
const int COLOR_KEY_G = 0;
const int COLOR_KEY_B = 0;

const int RENDER_DRAW_COLOR = 255;




typedef struct Input {
	int left_;
	int right_;
	int up_;
	int down_;
	int shoot_;

};
namespace SDLCommonFunc {
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
	bool Random();
	void SaveToFile(const std::string& text, const long long& score);
	std::string getTopScores(const std::string& filename, int topN = 10);
}
#endif