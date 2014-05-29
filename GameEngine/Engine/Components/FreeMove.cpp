#include "FreeMove.h"

FreeMove::FreeMove(float speed, int forwardKey, int backwardKey, int leftKey, int rightKey) :
	speed(speed), forwardKey(forwardKey), backwardKey(backwardKey), leftKey(leftKey), rightKey(rightKey)
{
}

void FreeMove::input(float delta)
{
	float moveAmount = speed * delta;

	if(Input::getKey(forwardKey))
	{
		move(getTransform().getRotation().getForward(), moveAmount);
	}

	if(Input::getKey(backwardKey))
	{
		move(getTransform().getRotation().getBackward(), moveAmount);
	}

	if(Input::getKey(leftKey))
	{
		move(getTransform().getRotation().getLeft(), moveAmount);
	}

	if(Input::getKey(rightKey))
	{
		move(getTransform().getRotation().getRight(), moveAmount);
	}
}

void FreeMove::move(const Vector3& direction, float amount)
{
	getTransform().setPosition(getTransform().getPosition() + (direction * amount));
}