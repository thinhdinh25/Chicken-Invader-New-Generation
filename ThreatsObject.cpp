#include <iostream>
#include "ThreatsObject.h"
#include <string> 
#include <cmath>

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
	move_direction_ = RIGHT;
	exist_time_ = 0;
	is_boss = false;
	hit_count = 0;
	hit_time_ = 0;
	Gravity_Speed = 0;

	int spawn_time = SDL_GetTicks();
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

void ThreatsObject::set_egg_broken_clips() {
	for (int i = 0; i < 8; i++) {
		frame_clip_[i].x = i * width_frame_;
		frame_clip_[i].y = 0;
		frame_clip_[i].w = width_frame_;
		frame_clip_[i].h = height_frame_;
	}
}

void ThreatsObject::SpawnThreats(SDL_Renderer* des, int number) {
	y_val_ = 50;
	is_boss = false;

	if (p_threat_list_.size() == 0) {
		int i = 1;
		while (i < number) {
			ThreatsObject* p_threat = new ThreatsObject();
			p_threat->LoadImg("img//chicken_sprite.png", des);

			p_threat->set_x_pos(x_val_);
			p_threat->set_y_pos(y_val_);
			p_threat->set_clips();
			p_threat->frame_ = rand() % 28;
			p_threat_list_.push_back(p_threat);

			x_val_ += (p_threat->get_width_frame() * SCALE_NUMBER + 15);
			if (x_val_ > SCREEN_WIDTH * 2 / 3) {
				x_val_ = 0;
				y_val_ += (p_threat->get_height_frame() * SCALE_NUMBER + 3);
				++i;
			}
		}

	}
}

void ThreatsObject::SpawnThreatsTriangle(SDL_Renderer* des, int number) {
	y_val_ = 50; 
	int row = 1;  
	is_boss = false;

	int created = 0; 
	ThreatsObject* p_threat = new ThreatsObject();
	p_threat->LoadImg("img//chicken_sprite.png", des);
	int threat_width = p_threat->get_width_frame();
	delete p_threat;
	if (p_threat_list_.size() == 0) {
		while (created < number) {
			int start_x = SCREEN_WIDTH / 2 - (row * (threat_width * SCALE_NUMBER + 10)) / 2;
			int x_val_ = start_x;

			for (int j = 0; j < row && created < number; ++j) {
				ThreatsObject* p_threat = new ThreatsObject();
				if (!p_threat->LoadImg("img//chicken_sprite.png", des)) {
					delete p_threat;
					continue;
				}

				p_threat->set_x_pos(x_val_);
				p_threat->set_y_pos(y_val_);
				p_threat->set_clips();
				p_threat->frame_ = rand() % 28;
				p_threat_list_.push_back(p_threat);

				x_val_ += (threat_width * SCALE_NUMBER + 10);
				++created;
			}

			y_val_ += (50 * SCALE_NUMBER + 20);
			++row; 
		}
	}
}

void ThreatsObject::SpawnThreatsCircle(SDL_Renderer* des, int number) {
	if (p_threat_list_.size() > 0) return;
	is_boss = false;
	const int centerX = SCREEN_WIDTH / 2;
	const int centerY = SCREEN_HEIGHT / 3;
	const int radius = 300;
	const double angleStep = (2 * M_PI) / number;

	for (int i = 0; i < number; ++i) {
		double angle = i * angleStep;
		int x_pos = centerX + radius * cos(angle);
		int y_pos = centerY + radius * sin(angle);

		ThreatsObject* p_threat = new ThreatsObject();
		if (!p_threat->LoadImg("img//chicken_sprite.png", des)) {
			delete p_threat;
			continue; 
		}

		p_threat->set_x_pos(x_pos);
		p_threat->set_y_pos(y_pos);
		p_threat->set_clips();
		p_threat->frame_ = rand() % 28;
		p_threat_list_.push_back(p_threat);

	}
}

void ThreatsObject::SpawnBoss(SDL_Renderer* des) {
	if (p_threat_list_.size() > 0) return;
	is_boss = true;
	ThreatsObject* p_threat = new ThreatsObject();
	p_threat->LoadImg("img//boss_spritesheet.png", des);
	p_threat->set_x_pos(SCREEN_WIDTH/2 - p_threat->get_width_frame());
	p_threat->set_y_pos(SCREEN_HEIGHT / 2 - p_threat->get_height_frame() - 50);
	p_threat->set_clips();
	p_threat->frame_ = rand() % 28;
	p_threat_list_.push_back(p_threat);
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
				ThreatsObject* egg_broken = new ThreatsObject();
				egg_broken->LoadImg("img//egg_broken_sprite.png",des);
				egg_broken->set_width_frame(egg_broken->get_width_frame() * 10 / 8);
				egg_broken->set_egg_broken_clips();
				egg_broken->set_x_pos(p_bullet->GetRect().x);
				egg_broken->set_y_pos(p_bullet->GetRect().y);
				egg_broken->exist_time_ = SDL_GetTicks();
				broken_egg_list_.push_back(egg_broken);
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
	if (rect_.x >= x_border || rect_.y < 0 || rect_.y >= y_border-30) {
		is_move_ = false;
	}
}

void ThreatsObject::HandleBrokenEgg(SDL_Renderer* des) {
	for (unsigned int i = 0; i < broken_egg_list_.size();i++) {
		ThreatsObject* broken_egg = broken_egg_list_.at(i);
		broken_egg->Show(des);
		broken_egg->update_frame(min(21, broken_egg->get_frame() + 1));
		if (SDL_GetTicks() - broken_egg->exist_time_ >= 3000) {
			broken_egg_list_.erase(broken_egg_list_.begin() + i);
			if (broken_egg != NULL) {
				delete broken_egg;
				broken_egg = NULL;
			}
		}
	}
}


void ThreatsObject::HandleAnimation(SDL_Renderer* des) {
	for (unsigned int i = 0; i < p_threat_list_.size(); i++) {
		ThreatsObject* p_threat = p_threat_list_.at(i);
		if (p_threat->get_x_pos() + p_threat->get_width_frame() >= SCREEN_WIDTH) {
			move_direction_ = LEFT;
		}
		else if (p_threat->get_x_pos() <= 0) {
			move_direction_ = RIGHT;
		}
		if (move_direction_ == RIGHT) {
			p_threat->set_x_pos(p_threat->get_x_pos() + 1);
		}
		else {
			p_threat->set_x_pos(p_threat->get_x_pos() - 1);
		}
		p_threat->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
		p_threat->Show(des);
		//threat bullet spawn
		if (SDL_GetTicks() - p_threat->spawn_time >= 1000) {
			if (SDLCommonFunc::Random() && threat_bullet_list_.size() < 7 && threat_bullet_list_.size() < p_threat_list_.size()) {
				if (is_boss) {
					int a = 5;
					int xVal = 0;
					for (int i = 0; i < a; i++) {
						Mix_PlayChannel(-1, Mix_LoadWAV("Sound//egg_spawn.wav"), 0);
						ThreatsObject* obj_threat_bullet = new ThreatsObject();
						obj_threat_bullet->LoadImg("img//egg.png", des);
						obj_threat_bullet->SetRect(p_threat->get_x_pos() + p_threat->get_width_frame() / 2 + xVal, p_threat->get_y_pos() + p_threat->get_height_frame());
						obj_threat_bullet->set_y_val(rand() % 6 + 3 + Gravity_Speed);
						threat_bullet_list_.push_back(obj_threat_bullet);
						if (move_direction_ == RIGHT) {
							xVal += 20;
						}
						else xVal -= 20;
					}
				}
				else {
					Mix_PlayChannel(-1, Mix_LoadWAV("Sound//egg_spawn.wav"), 0);
					ThreatsObject* obj_threat_bullet = new ThreatsObject();
					obj_threat_bullet->LoadImg("img//egg.png", des);
					obj_threat_bullet->SetRect(p_threat->get_x_pos() + p_threat->get_width_frame() / 2, p_threat->get_y_pos() + p_threat->get_height_frame());
					obj_threat_bullet->set_y_val(rand() % 6 + 3 + Gravity_Speed);
					threat_bullet_list_.push_back(obj_threat_bullet);
				}

			}
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

void ThreatsObject::HandleExplosion(SDL_Renderer* des) {
	for (unsigned int i = 0; i < threat_explosion_.size(); ++i) {

	}
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

void ThreatsObject::RemoveBullet(const int& idx) {
	unsigned int size = threat_bullet_list_.size();
	if (size > 0 && idx < size) {
		ThreatsObject* p_threat = threat_bullet_list_.at(idx);
		threat_bullet_list_.erase(threat_bullet_list_.begin() + idx);
		if (p_threat) {
			delete p_threat;
			p_threat = NULL;
		}
	}
}

void ThreatsObject::RemoveAllBullet() {
	for (int i = static_cast<int>(threat_bullet_list_.size()) - 1; i >= 0; i--) {
			ThreatsObject* p_threat = threat_bullet_list_.at(i);
			threat_bullet_list_.erase(threat_bullet_list_.begin() + i);
			if (p_threat) {
				delete p_threat;
				p_threat = NULL;
			}
		}
	}
	
void ThreatsObject::RemoveAllThreat() {
	for (int i = static_cast<int>(p_threat_list_.size()) - 1; i >= 0; i--) {
		ThreatsObject* p_threat = p_threat_list_.at(i);
		p_threat_list_.erase(p_threat_list_.begin() + i);
		if (p_threat) {
			delete p_threat;
			p_threat = NULL;
		}
	}
}