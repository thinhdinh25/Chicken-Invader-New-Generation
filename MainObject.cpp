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
	frame_num = 8;
	is_shown = 1;
	dead_time = 0;
	spawn_time = SDL_GetTicks();
}

MainObject::~MainObject() {

}



void MainObject::SpawnPlayer(SDL_Renderer* des) {
	x_pos_ = SCREEN_WIDTH / 2;
	y_pos_ = SCREEN_HEIGHT * 4 / 5;

	LoadImg("img//character.png", des);
	set_clips();

	spawn_time = SDL_GetTicks();
	p_shield = new MainObject();
	p_shield->frame_num = 11;
	p_shield->LoadImg("img//shield_spritesheet.png", des);
	p_shield->set_clips();

	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.shoot_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
}



bool MainObject::LoadImg(std::string path, SDL_Renderer* screen) {
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

void MainObject::set_clips() {
	if (width_frame_ > 0 && height_frame_ > 0) {
		for (int i = 0; i < frame_num; i++) {
			frame_clip_[i].x = i * width_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}
	}
}

void MainObject::Show(SDL_Renderer* des) {
	if (input_type_.left_ == 1) {
		frame_--;
	}
	else if (input_type_.right_ == 1) {
		frame_++;
	}
	else if (status_ == WALK_LEFT) frame_ = 3;
	else if (status_ == WALK_RIGHT) frame_ = 4;
	else frame_ = 3;

	if (status_ == WALK_LEFT && frame_ < 0) frame_ = 0;
	else if (status_ == WALK_RIGHT && frame_ >= 8) frame_ = 7;
	

	rect_.x = x_pos_;
	rect_.y = y_pos_;


	if (SDL_GetTicks() - spawn_time <= 3000) {
		SDL_Rect* current_clip = p_shield->frame_clip_;

		SDL_Rect renderQuad = { rect_.x - width_frame_/2-2, rect_.y -  height_frame_ / 2 - 20, p_shield->width_frame_ * SCALE_NUMBER, p_shield->height_frame_ * SCALE_NUMBER };

		SDL_RenderCopy(des, p_shield->p_object_, current_clip, &renderQuad);

		p_shield->frame_ = min(10, p_shield->frame_ + 1);
	}


	SDL_Rect* current_clip = &frame_clip_[frame_];

	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_*SCALE_NUMBER, height_frame_*SCALE_NUMBER };

	if(is_shown == 1) SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen) {
	static bool space_held = false;

	if (events.type == SDL_KEYDOWN) {
		switch (events.key.keysym.sym) {
		case SDLK_RIGHT:
			status_ = WALK_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
			break;
		case SDLK_LEFT:
			status_ = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
			break;
		case SDLK_UP:
			status_ = WALK_UP;
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 1;
			input_type_.down_ = 0;
			break;
		case SDLK_DOWN:
			status_ = WALK_DOWN;
			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 1;
			break;
		case SDLK_SPACE:
			if (!space_held) {
				if (SDL_GetTicks() - dead_time >= 2000) {
					space_held = true;
					BulletObject* p_bullet = new BulletObject();
					p_bullet->LoadImg("img//shot.png", screen);
					p_bullet->SetRect(this->rect_.x + width_frame_ / 2 - 16, rect_.y);
					p_bullet->set_y_val(3);
					p_bullet->set_is_move(true);
					p_bullet_list_.push_back(p_bullet);
				}
			}
			break;
		default:
			break;
		}
	}
	else if (events.type == SDL_KEYUP) {
		switch (events.key.keysym.sym) {
		case SDLK_RIGHT:
			input_type_.right_ = 0;
			break;
		case SDLK_LEFT:
			input_type_.left_ = 0;
			break;
		case SDLK_UP:
			input_type_.up_ = 0;
			break;
		case SDLK_DOWN:
			input_type_.down_ = 0;
			break;
		case SDLK_SPACE:
			space_held = false;
			break;
		}
	}
}


void MainObject::HandleBullet(SDL_Renderer* des) {
	for (int i = p_bullet_list_.size() - 1; i >= 0; i--) {
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


void MainObject::DoPlayer(SDL_Renderer* des) {
	x_val_ = 0;
	y_val_ = 0;
	if (is_shown == 1) {
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

		x_pos_ = min(x_pos_, SCREEN_WIDTH - width_frame_);
		y_pos_ = min(y_pos_, SCREEN_HEIGHT - height_frame_ - 50);

		x_pos_ = max(x_pos_, 0);
		y_pos_ = max(y_pos_, 0);
	}
	else {
		if (SDL_GetTicks() - dead_time >= 2000) { SpawnPlayer(des); is_shown = 1; }
		else {
			p_explosion->Show(des);
			p_explosion->update_frame(min(10, p_explosion->get_frame()+1));
		}
		
	}
}


void MainObject::ReSpawn(SDL_Renderer* des) {
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
	is_shown = 0;
	dead_time = SDL_GetTicks();
	SetRect(1 * SCREEN_WIDTH, 1 * SCREEN_HEIGHT);
	if (p_explosion) {
		delete p_explosion;
		p_explosion = nullptr;
	}
	p_explosion = new Explosion();
	p_explosion->update_frame(0);
	p_explosion->set_x_pos(x_pos_- width_frame_/2);
	p_explosion->set_y_pos(y_pos_ - height_frame_/2-10);
	p_explosion->LoadImg("img//explosion_spritesheet.png",des);
	p_explosion->set_clips();
}

void MainObject::ResetPlayer() {
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
	spawn_time = SDL_GetTicks();
}
