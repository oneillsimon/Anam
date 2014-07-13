#ifndef FREELOOK_H
#define FREELOOK_H

#include "../Core/Math3D.h"
#include "../Core/Input.h"
#include "GameComponent.h"

class FreeLook : public GameComponent
{
private:
	float m_sensitivity;
	int m_unlockMouseKey;

public:
	FreeLook(float sensitivity = 0.5f, int unlockMouseKey = Input::KEY_ESCAPE);
	virtual void input(float delta);
};

#endif