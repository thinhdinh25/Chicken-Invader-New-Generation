#include <iostream>
#include "ThreatsObject.h"
#include <string> 

ThreatsObject::ThreatsObject() {
	width_frame_ = 0;
	height_frame_ = 0;
	x_pos_ = 0.0;
	y_pos_ = 0.0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	frame_ = 0;
	frame_increase_ = true;
}

ThreatsObject::~ThreatsObject() {

}

bool ThreatsObject::LoadImg(std::string path, SDL_Renderer* screen) {
	if (p_object_ != nullptr) {
		SDL_DestroyTexture(p_object_);  // Remove old object to prevent memory leakage
		p_object_ = nullptr;
	}
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true) {
		width_frame_ = rect_.w;
		height_frame_ = rect_.h;
	}
	return ret;
}

void ThreatsObject::LoadAnimationFrames(SDL_Renderer* renderer) {
	for (int i = 0; i < 10; i++) {  
		std::string path = "img//chicken_img/chickenb_" + std::to_string(i) + ".bmp";
		ThreatsObject p_object = null;

		if (new_texture) {
			animation_frames_.push_back(new_texture);
		}
	}
}

void ThreatsObject::HandleMove(const int& x_border, const int& y_border) {

}


void ThreatsObject::SpawnThreats(SDL_Renderer* des, int number) {
	if (p_threat_list_.size() == 0) {
		for (int i = 0; i < number; i++) {
			ThreatsObject* p_threat = new ThreatsObject();
			p_threat->LoadImg("img//chicken_img/chickenb_0.bmp", des);
			p_threat->set_x_pos(x_val_);
			p_threat->set_y_pos(y_val_);
			p_threat_list_.push_back(p_threat);

			x_val_ += (p_threat->get_width_frame() * SCALE_NUMBER + 5);
			if (x_val_ > 700) {
				x_val_ = 0;
				y_val_ += (p_threat->get_height_frame() * SCALE_NUMBER + 20);
			}
		}
	}
}


void ThreatsObject::HandleAnimation(SDL_Renderer* des){
	std::string path = "img//chicken_img/chickenb_" + std::to_string(frame_/3) + ".bmp";
	for (unsigned int i = 0; i < p_threat_list_.size(); i++) {
		ThreatsObject* p_threat = p_threat_list_.at(i);
		p_threat->LoadImg(path, des);
		p_threat->Show(des);
	}
	if (frame_increase_)
		frame_++;
	else
		frame_--;

	if (frame_ > 27)
		frame_increase_ = false;
	else if (frame_ <= 1)
		frame_increase_ = true;
}

void ThreatsObject::Show(SDL_Renderer* des) {
	rect_.x = x_pos_;
	rect_.y = y_pos_;

	SDL_Rect* current_clip = NULL;

	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_*SCALE_NUMBER, height_frame_*SCALE_NUMBER };

	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

}

