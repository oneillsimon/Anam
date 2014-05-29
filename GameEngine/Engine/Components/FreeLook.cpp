#include "../Rendering/Window.h"
#include "../Core/GameMath.h"
#include "FreeLook.h"

bool mouseLocked = false;

FreeLook::FreeLook(float sensitivity, int unlockMouseKey)
{
	m_sensitivity = sensitivity;
	m_unlockMouseKey = unlockMouseKey;
}

void FreeLook::input(float delta)
{
	if(Input::getKey(m_unlockMouseKey))
	{
		Input::setCursor(true);
		mouseLocked = false;
	}

	if(mouseLocked)
	{
		Vector2 centrePosition = Vector2((float)(Window::getWidth() / 2), (float)(Window::getHeight() / 2));
		Vector2 deltaPos = Input::getMousePosition() - centrePosition;

		bool rotX = deltaPos.getY() != 0;
		bool rotY = deltaPos.getX() != 0;

		if(rotY)
		{
			getTransform().rotate(Vector3(0, 1, 0), (float)GameMath::toRadians(deltaPos.getX() * m_sensitivity));
		}

		if(rotX)
		{
			getTransform().rotate(getTransform().getRotation().getRight(), (float)GameMath::toRadians(deltaPos.getY() * m_sensitivity));
		}

		if(rotY || rotX)
		{
			Input::setMousePosition(centrePosition);
			Quaternion q = getTransform().getRotation().normalised();

			fprintf(stdout, "%.4f, %.4f, %.4f, %.4f\n", q.getX(), q.getY(), q.getZ(), q.getW());
		}
	}

	if(Input::getMouseDown(Input::LEFT_MOUSE))
	{
		Vector2 centrePosition = Vector2((float)(Window::getWidth() / 2), (float)(Window::getHeight() / 2));
		Input::setCursor(false);
		Input::setMousePosition(centrePosition);
		mouseLocked = true;
	}
}