#ifndef THREATS_OBJECT_H_
#define THREATS_OBJECT_H_

#include <vector>
#include "CommonFunc.h"
#include "BaseObject.h"

#define THREAT_FRAME_NUM 8

class ThreatsObject : public BaseObject {

public:
	ThreatsObject();
	~ThreatsObject();


	void set_x_val(const float& xVal) { x_val_ = xVal; }
	void set_y_val(const float& yVal) { y_val_ = yVal; }
	void set_x_pos(const float& xPos) { x_pos_ = xPos; }
	void set_y_pos(const float& yPos) { y_pos_ = yPos; }
	float get_x_pos() const { return x_pos_; }
	float get_y_pos() const { return y_pos_; }

	void SetMapXY(const int& xMap, const int& yMap) { map_x = xMap; map_y = yMap; }

	void Set_clips();
	virtual bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	int get_width_frame() const { return width_frame_; }
	int get_height_frame() const { return height_frame_; }
	void DoPlayer();
	void HandleAnimation(SDL_Renderer* des);
	void SpawnThreats(SDL_Renderer* des, int number);
	void HandleMove(const int& x_border, const int& y_border);

	void LoadAnimationFrames(SDL_Renderer* renderer);

private:
	std::vector<ThreatsObject*> p_threat_list_;
	int map_x;
	int map_y;
	float x_val_;
	float y_val_;
	float x_pos_;
	float y_pos_;

	SDL_Rect frame_clip_[THREAT_FRAME_NUM];
	int width_frame_;
	int height_frame_;
	int frame_;
	bool frame_increase_;



};

#endif
