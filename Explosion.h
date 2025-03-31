#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "CommonFunc.h"
#include "BaseObject.h"

class Explosion : public BaseObject {

public:
	Explosion();
	~Explosion();
	virtual bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void set_clips();
	void set_x_pos(int xVal) { x_pos_ = xVal; }
	void set_y_pos(int yVal) { y_pos_ = yVal; }
	void update_frame(int frame) { frame_ = frame; }
	int get_frame() const { return frame_; }

private:
	int width_frame_;
	int height_frame_;
	int x_pos_;
	int y_pos_;

	SDL_Rect frame_clip_[20];
	int frame_;
	int frame_num;
};



#endif