#ifndef FREEMOVE_H
#define FREEMOVE_H

#include "GameComponent.h"

class FreeMove : public GameComponent
{
private:

	float speed;
	int forwardKey;
	int backwardKey;
	int leftKey;
	int rightKey;

	void move(const Vector3& direction, float amount);

public:
	FreeMove(float speed = 10.0f, int forwardKey = Input::KEY_W, int backwardKey = Input::KEY_S, int leftKey = Input::KEY_A, int rightKey = Input::KEY_D);
	virtual void input(const Input& input, float delta);
};

#endif