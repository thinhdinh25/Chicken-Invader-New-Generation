#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include <vector>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include "ThreatsObject.h"
#include "Explosion.h"


#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 5

class MainObject : public BaseObject {

public:
	MainObject();
	~MainObject();

	enum WalkType {
		WALK_RIGHT = 0,
		WALK_LEFT = 1,
		WALK_UP = 2,
		WALK_DOWN = 3,
	};

	virtual bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
	void set_clips();
	void DoPlayer(SDL_Renderer* des);
	void set_bullet_list(std::vector<BulletObject*> bullet_list) {
		p_bullet_list_ = bullet_list;
	}
	std::vector<BulletObject*> get_bullet_list() const { return p_bullet_list_; }
	void HandleBullet(SDL_Renderer* des);
	void RemoveBullet(const int& idx);
	int get_width_frame() const { return width_frame_; }
	int get_height_frame() const { return height_frame_;}
	void set_x_pos(float xVal) { x_pos_ = xVal; }
	void set_y_pos(float yVal) { y_pos_ = yVal; }

	void SpawnPlayer(SDL_Renderer* des);

	void SetSpawnTime(long time) { spawn_time = time; }
	int GetSpawnTime() const { return spawn_time; }

	void ReSpawn(SDL_Renderer* des);

	int get_is_shown() const { return is_shown; }

private:
	std::vector<BulletObject*> p_bullet_list_;
	float x_val_;
	float y_val_;

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[20];
	Input input_type_;
	int spawn_time;
	int dead_time;
	
	int frame_;
	int frame_num;
	int status_;
	MainObject* p_shield;
	Explosion* p_explosion;
	int is_shown;
};


#endif // !MAIN_OBJECT_H_
