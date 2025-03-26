#include "BulletObject.h"
#include "iostream"
BulletObject::BulletObject() {
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = false;

}

BulletObject::~BulletObject() {

}

void BulletObject::HandleMove(const int &x_border, const int &y_border) {
	rect_.x += x_val_;
	rect_.y -= y_val_;
	if (rect_.x > x_border || rect_.y < 0) {
		is_move_ = false;
	}
}