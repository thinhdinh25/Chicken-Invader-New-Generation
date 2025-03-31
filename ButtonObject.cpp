#include "ButtonObject.h"

ButtonObject::ButtonObject() {
	status_ = UNCLICKED_;
	width_frame_ = 0;
	height_frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	is_hovered = false;
}

ButtonObject::~ButtonObject() {

}

bool ButtonObject::LoadImg(std::string path, SDL_Renderer* screen) {
	if (p_object_ != nullptr) {
		SDL_DestroyTexture(p_object_);
		p_object_ = nullptr;
	}
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true) {
		width_frame_ = rect_.w ;
		height_frame_ = rect_.h;
	}
	return ret;
}

void ButtonObject::Show(SDL_Renderer* des) {
	rect_.x = x_pos_;
	rect_.y = y_pos_;

	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_ * SCALE_NUMBER, height_frame_ * SCALE_NUMBER };

	SDL_RenderCopy(des, p_object_, NULL, &renderQuad);

}

bool ButtonObject::is_hovered_(SDL_Event &g_event) {
	int px = g_event.button.x;
	int py = g_event.button.y;

	return (px >= x_pos_ && px <= x_pos_ + width_frame_ &&
		py >= y_pos_ && py <= y_pos_ + height_frame_);
}

bool ButtonObject::is_clicked_(SDL_Event& g_event) {
	int px = g_event.button.x;
	int py = g_event.button.y;

	return (px >= x_pos_ && px <= x_pos_ + width_frame_ &&
		py >= y_pos_ && py <= y_pos_ + height_frame_);
}

void ButtonObject::state_change(SDL_Renderer* des, bool state, std::string path_hover, std::string path_normal)
{	
	if (state != is_hovered) {
		is_hovered = state;
		if (is_hovered == true) {
			LoadImg(path_hover, des);
		}
		else {
			LoadImg(path_normal, des);
		}
	}
	
}
