#ifndef BUTTON_H_
#define BUTTON_H_
#include "CommonFunc.h"
#include "BaseObject.h"
#include <string>

class ButtonObject : public BaseObject {

public:
	enum CLICK_STATE {
		CLICKED_ = 1,
		UNCLICKED_ = 0,
	};
	ButtonObject();
	~ButtonObject();
	virtual bool LoadImg(std::string path, SDL_Renderer * screen);
	void Show(SDL_Renderer* des);
	void set_x_pos(int xVal) { x_pos_ = xVal; }
	void set_y_pos(int yVal) { y_pos_ = yVal; }
	int get_x_pos() const { return x_pos_; }
	int get_y_pos() const { return y_pos_; }
	int get_width_frame_() const { return width_frame_; }
	int get_height_frame_() const { return height_frame_; }

	bool is_hovered_(SDL_Event &g_event);
	bool is_clicked_(SDL_Event& g_event);
	void state_change(SDL_Renderer* des, bool state, std::string path_hover, std::string path_normal);
private:
	int status_;
	float width_frame_;
	float height_frame_;

	int x_pos_;
	int y_pos_;

	bool is_hovered;
};


#endif // !BUTTON_H_
