#ifndef MOVEMENT2D_H
#define MOVEMENT2D_H

#include "../../Core/GameComponent.h"

class Movement2D : public GameComponent
{
private:

	float speed;
	int upKey;
	int downKey;
	int leftKey;
	int rightKey;

	void move(const Vector3& direction, float amount);

public:
	Movement2D(float speed = 10.0f, int upKey = Input::KEY_I, int downKey = Input::KEY_K, int leftKey = Input::KEY_J, int rightKey = Input::KEY_L);
	virtual void input(const Input& input, float delta);
};

#endif