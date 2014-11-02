#include "Movement2D.h"

Movement2D::Movement2D(float speed, int forwardKey, int backwardKey, int leftKey, int rightKey) :
speed(speed), upKey(forwardKey), downKey(backwardKey), leftKey(leftKey), rightKey(rightKey)
{
}

void Movement2D::input(const Input& input, float delta)
{
	float moveAmount = speed * delta;

	if(input.getKey(upKey))
	{
		move(AXIS_Y, moveAmount);
	}

	if(input.getKey(downKey))
	{
		move(AXIS_Y * -1, moveAmount);
	}

	if(input.getKey(leftKey))
	{
		move(AXIS_X * -1, moveAmount);
	}

	if(input.getKey(rightKey))
	{
		move(AXIS_X, moveAmount);
	}
}

void Movement2D::move(const Vector3& direction, float amount)
{
	getTransform()->setPosition(getTransform()->getPosition() + (direction * amount));
}