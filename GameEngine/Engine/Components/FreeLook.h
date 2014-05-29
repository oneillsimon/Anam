#include "../Core/3DMath.h"
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