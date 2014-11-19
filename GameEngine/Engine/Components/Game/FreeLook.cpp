#include "FreeLook.h"

bool mouseLocked = false;

FreeLook::FreeLook(const Vector2 windowCentre, float sensitivity, int unlockMouseKey)
{
	m_windowCentre = windowCentre;
	m_sensitivity = sensitivity;
	m_unlockMouseKey = unlockMouseKey;
}

void FreeLook::input(const Input& input, float delta)
{
	if(input.getKey(m_unlockMouseKey))
	{
		input.setCursor(true);
		mouseLocked = false;
	}

	if(mouseLocked)
	{
		Vector2 delta = input.getMousePosition() - m_windowCentre;

		bool rotY = delta.getX() != 0;
		bool rotX = delta.getY() != 0;

		if(rotY)
		{
			getTransform()->rotate(Vector3(0.0f, 1.0f, 0.0f), (float)toRadians(delta.getX() * m_sensitivity));
		}

		if(rotX)
		{
			getTransform()->rotate(getTransform()->getRotation().getRight(), (float)toRadians(delta.getY() * m_sensitivity));
		}

		if(rotX || rotY)
		{
			input.setCursor(false);
			input.setMousePosition(m_windowCentre);
			mouseLocked = true;
		}
	}

	if(input.getMouseDown(Input::MOUSE_LEFT))
	{
		input.setCursor(false);
		input.setMousePosition(m_windowCentre);
		mouseLocked = true;
	}
}