#ifndef FREELOOK_H
#define FREELOOK_H

#include "GameComponent.h"

class FreeLook : public GameComponent
{
private:
	float m_sensitivity;
	int m_unlockMouseKey;
	Vector2 m_windowCentre;

public:
	FreeLook(const Vector2 windowCentre, float sensitivity = 0.5f, int unlockMouseKey = Input::KEY_ESCAPE);
	virtual void input(const Input& input, float delta);
};

#endif