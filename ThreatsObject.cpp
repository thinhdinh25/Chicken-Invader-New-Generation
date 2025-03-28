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
	is_move_ = true;
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
		width_frame_ = rect_.w / THREAT_FRAME_NUM;
		height_frame_ = rect_.h;
	}
	return ret;
}

void ThreatsObject::set_clips() {
	for (int i = 0; i < THREAT_FRAME_NUM; i++) {
		frame_clip_[i].x = i * width_frame_;
		frame_clip_[i].y = 0;
		frame_clip_[i].w = width_frame_;
		frame_clip_[i].h = height_frame_;
	}
}


void ThreatsObject::SpawnThreats(SDL_Renderer* des, int number) {
	if (p_threat_list_.size() == 0) {
		for (int i = 0; i < number; i++) {
			ThreatsObject* p_threat = new ThreatsObject();
			p_threat->LoadImg("img//chicken_sprite.png", des);
			p_threat->set_x_pos(x_val_);
			p_threat->set_y_pos(y_val_);
			p_threat->set_clips();
			p_threat_list_.push_back(p_threat);

			x_val_ += (p_threat->get_width_frame() * SCALE_NUMBER + 5);
			if (x_val_ > 700) {
				x_val_ = 0;
				y_val_ += (p_threat->get_height_frame() * SCALE_NUMBER + 20);
			}
		}
		
	}
}
void ThreatsObject::HandleThreatBullet(SDL_Renderer* des) {
	for (unsigned int i = 0; i < threat_bullet_list_.size(); i++) {
		ThreatsObject* p_bullet = threat_bullet_list_.at(i);
		if (p_bullet != NULL) {
			if (p_bullet->get_is_move() == true) {
				p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
				p_bullet->Render(des);
			}
			else {
				threat_bullet_list_.erase(threat_bullet_list_.begin() + i);
				if (p_bullet != NULL) {
					delete p_bullet;
					p_bullet = NULL;
				}
			}
		}
	}
}

void ThreatsObject::HandleMove(const int& x_border, const int& y_border) {
	rect_.x += x_val_;
	rect_.y += y_val_;
	if (rect_.x > x_border || rect_.y < 0 || rect_.y > y_border) {
		is_move_ = false;
	}
}

void ThreatsObject::HandleAnimation(SDL_Renderer* des){
	for (unsigned int i = 0; i < p_threat_list_.size(); i++) {
		ThreatsObject* p_threat = p_threat_list_.at(i);
		p_threat->Show(des);
		//threat bullet spawn
		if (SDLCommonFunc::Random() && threat_bullet_list_.size() < 10) {
			ThreatsObject* obj_threat_bullet = new ThreatsObject();
			obj_threat_bullet->LoadImg("img//egg.bmp", des);
			obj_threat_bullet->SetRect(p_threat->get_x_pos() + p_threat->get_width_frame() / 2, p_threat->get_y_pos() + p_threat->get_height_frame());
			obj_threat_bullet->set_y_val(5);
			threat_bullet_list_.push_back(obj_threat_bullet);
		}

		int current_frame_ = p_threat->get_frame();
		if (p_threat->get_frame_increase())
			current_frame_++;
		else
			current_frame_--;

		if (current_frame_ > 27) {
			current_frame_ = 27;
			p_threat->update_frame_increase_(false);
		}
		else if (current_frame_ <= 1) {
			current_frame_ = 1;
			p_threat->update_frame_increase_(true);
		}
		p_threat->update_frame(current_frame_);


	}
}

void ThreatsObject::Show(SDL_Renderer* des) {
	rect_.x = x_pos_;
	rect_.y = y_pos_;
	SDL_Rect* current_clip = &frame_clip_[frame_/3];
	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_*SCALE_NUMBER, height_frame_*SCALE_NUMBER };

	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}

void ThreatsObject::RemoveThreat(const int& idx) {
	unsigned int size = p_threat_list_.size();
	if (size > 0 && idx < size) {
		ThreatsObject* p_threat = p_threat_list_.at(idx);
		p_threat_list_.erase(p_threat_list_.begin() + idx);
		if (p_threat) {
			delete p_threat;
			p_threat = NULL;
		}
	}
}