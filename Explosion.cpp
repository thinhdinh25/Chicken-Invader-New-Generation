#include "Explosion.h"

Explosion::Explosion() {
	width_frame_ = 0;
	height_frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;

	frame_ = 0;
	frame_num = 11;
}

Explosion::~Explosion() {

}

bool Explosion::LoadImg(std::string path, SDL_Renderer* screen) {
	if (p_object_ != nullptr) {
		SDL_DestroyTexture(p_object_);  // Remove old object to prevent memory leakage
		p_object_ = nullptr;
	}
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true) {
		width_frame_ = rect_.w / frame_num;
		height_frame_ = rect_.h;
	}
	return ret;
}

void Explosion::set_clips() {
	if (width_frame_ > 0 && height_frame_ > 0) {
		for (int i = 0; i < frame_num; i++) {
			frame_clip_[i].x = i * width_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}
	}
}

void Explosion::Show(SDL_Renderer* des) {
	rect_.x = x_pos_;
	rect_.y = y_pos_;

	SDL_Rect* current_clip = &frame_clip_[frame_];

	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_ * SCALE_NUMBER, height_frame_ * SCALE_NUMBER };

	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

}