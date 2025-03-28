#include "MainObject.h"
#include <iostream>

MainObject::MainObject() {
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_LEFT;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.shoot_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
}

MainObject::~MainObject() {

}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen) {
	if (p_object_ != nullptr) {
		SDL_DestroyTexture(p_object_);  // Remove old object to prevent memory leakage
		p_object_ = nullptr;
	}
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true) {
		width_frame_ = rect_.w / 8;
		height_frame_ = rect_.h;
	}
	return ret;
}

void MainObject::set_clips() {
	if (width_frame_ > 0 && height_frame_ > 0) {
		if (status_ == WALK_LEFT) {
			frame_clip_[3].x = 0;
			frame_clip_[3].y = 0;
			frame_clip_[3].w = width_frame_;
			frame_clip_[3].h = height_frame_;

			frame_clip_[2].x = width_frame_;
			frame_clip_[2].y = 0;
			frame_clip_[2].w = width_frame_;
			frame_clip_[2].h = height_frame_;

			frame_clip_[1].x = 2 * width_frame_;
			frame_clip_[1].y = 0;
			frame_clip_[1].w = width_frame_;
			frame_clip_[1].h = height_frame_;

			frame_clip_[0].x = 3 * width_frame_;
			frame_clip_[0].y = 0;
			frame_clip_[0].w = width_frame_;
			frame_clip_[0].h = height_frame_;
		}
		else {
			frame_clip_[0].x = 4 * width_frame_;
			frame_clip_[0].y = 0;
			frame_clip_[0].w = width_frame_;
			frame_clip_[0].h = height_frame_;

			frame_clip_[1].x = 5 * width_frame_;
			frame_clip_[1].y = 0;
			frame_clip_[1].w = width_frame_;
			frame_clip_[1].h = height_frame_;

			frame_clip_[2].x = 6 * width_frame_;
			frame_clip_[2].y = 0;
			frame_clip_[2].w = width_frame_;
			frame_clip_[2].h = height_frame_;

			frame_clip_[3].x = 7 * width_frame_;
			frame_clip_[3].y = 0;
			frame_clip_[3].w = width_frame_;
			frame_clip_[3].h = height_frame_;
		}

	}
}

void MainObject::Show(SDL_Renderer* des) {
	LoadImg("img//character.png", des);
	set_clips();
	if (input_type_.left_ == 1 || input_type_.right_ == 1) {
		frame_++;
	}
	else frame_ = 0;
	
	if (frame_ >= 4) frame_ = 3;

	rect_.x = x_pos_;
	rect_.y = y_pos_;

	SDL_Rect* current_clip = &frame_clip_[frame_];

	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_*SCALE_NUMBER, height_frame_*SCALE_NUMBER };

	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen) {
	if (events.type == SDL_KEYDOWN) {
		switch (events.key.keysym.sym) {
		case SDLK_RIGHT:
			{
				status_ = WALK_RIGHT;
				input_type_.right_ = 1;
				input_type_.left_ = 0;
				input_type_.up_ = 0;
				input_type_.down_ = 0;
			}
			break;
		case SDLK_LEFT:
			{
				status_ = WALK_LEFT;
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				input_type_.up_ = 0;
				input_type_.down_ = 0;
			}
			break;
		case SDLK_UP:
		{
			status_ = WALK_UP;
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 1;
			input_type_.down_ = 0;
		}
		break;
		case SDLK_DOWN:
		{
			status_ = WALK_DOWN;
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 1;
		}
		break;
		case SDLK_SPACE:
		{
			BulletObject* p_bullet = new BulletObject();
			p_bullet->LoadImg("img//shot.bmp", screen);
			p_bullet->SetRect(this->rect_.x + width_frame_/2 - 10 , rect_.y + height_frame_);
			p_bullet->set_y_val(20);
			p_bullet->set_is_move(true);

			p_bullet_list_.push_back(p_bullet);
		}
		break;
		default:
			break;
		}
	}
	else if (events.type == SDL_KEYUP) {
		switch (events.key.keysym.sym) {
		case SDLK_RIGHT:
		{
			input_type_.right_ = 0;
			input_type_.left_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
		}
		break;
		case SDLK_LEFT:
		{
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
		}
		break;
		case SDLK_UP:
		{
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
		}
		break;
		case SDLK_DOWN:
		{
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
		}
		break;
		case SDLK_SPACE:
		{

		}
		break;
		}
	}
}

void MainObject::HandleBullet(SDL_Renderer* des) {
	for (unsigned int i = 0; i < p_bullet_list_.size(); i++) {
		BulletObject* p_bullet = p_bullet_list_.at(i);
		if (p_bullet != NULL) {
			if (p_bullet->get_is_move() == true) {
				p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
				p_bullet->Render(des);
			}
			else {
				p_bullet_list_.erase(p_bullet_list_.begin() + i);
				if (p_bullet != NULL) {
					delete p_bullet;
					p_bullet = NULL;
				}
			}
		}
	}
}

void MainObject::RemoveBullet(const int& idx) {
	unsigned int size = p_bullet_list_.size();
	if (size > 0 && idx < size) {
		BulletObject* p_bullet = p_bullet_list_.at(idx);
		p_bullet_list_.erase(p_bullet_list_.begin() + idx);
		if (p_bullet) {
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}


void MainObject::DoPlayer() {
	x_val_ = 0;
	y_val_ = 0;

	if (input_type_.left_ == 1) {
		x_val_ -= PLAYER_SPEED;
	}
	else if (input_type_.right_ == 1) {
		x_val_ += PLAYER_SPEED;
	}
	else if (input_type_.up_ == 1) {
		y_val_ -= PLAYER_SPEED;
	}
	else if (input_type_.down_ == 1) {
		y_val_ += PLAYER_SPEED;
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;

	x_pos_ = min(x_pos_, MAX_MAP_X);
	y_pos_ = min(y_pos_, MAX_MAP_Y);

	x_pos_ = max(x_pos_, 0);
	y_pos_ = max(y_pos_, 0);
}