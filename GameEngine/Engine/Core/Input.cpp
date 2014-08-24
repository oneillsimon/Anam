#include <cstring>
#include <SDL2/SDL.h>

#include "Input.h"
#include "SDLWrapper.h"

Input::Input(Window* window)
{
	m_mouseX = 0;
	m_mouseY = 0;
	m_window = window;

	memset(m_inputs, 0, NUM_KEYS * sizeof(bool));
	memset(m_downKeys, 0, NUM_KEYS * sizeof(bool));
	memset(m_upKeys, 0, NUM_KEYS * sizeof(bool));

	memset(m_mouseInputs, 0, NUM_MOUSE_BUTTONS * sizeof(bool));
	memset(m_downMouse, 0, NUM_MOUSE_BUTTONS * sizeof(bool));
	memset(m_upMouse, 0, NUM_MOUSE_BUTTONS * sizeof(bool));
}

bool Input::getKey(int keyCode) const
{
	return m_inputs[keyCode];
}

bool Input::getKeyDown(int keyCode) const
{
	return m_downKeys[keyCode];
}

bool Input::getKeyUp(int keyCode) const
{
	return m_upKeys[keyCode];
}

bool Input::getMouse(int button) const
{
	return m_mouseInputs[button];
}

bool Input::getMouseDown(int button) const
{
	return m_downMouse[button];
}

bool Input::getMouseUp(int button) const
{
	return m_upMouse[button];
}

Vector2 Input::getMousePosition() const
{
	return Vector2((float)m_mouseX, (float)m_mouseY);
}

void Input::setKey(int keyCode, bool value)
{
	m_inputs[keyCode] = value;
}

void Input::setKeyDown(int keyCode, bool value)
{
	m_downKeys[keyCode] = value;
}

void Input::setKeyUp(int keyCode, bool value)
{
	m_upKeys[keyCode] = value;
}

void Input::setMouse(int mouseButton, bool value)
{
	m_mouseInputs[mouseButton] = value;
}

void Input::setMouseDown(int mouseButton, bool value)
{
	m_downMouse[mouseButton] = value;
}

void Input::setMouseUp(int mouseButton, bool value)
{
	m_upMouse[mouseButton] = value;
}

void Input::setMousePosition(Vector2 position) const
{
	SDLSetMousePosition((int)position.getX(), (int)position.getY());
}

void Input::setMouseX(int x)
{
	m_mouseX = x;
}

void Input::setMouseY(int y)
{
	m_mouseY = y;
}

void Input::setCursor(bool value) const
{
	if(value)
	{
		SDL_ShowCursor(1);
	}
	else
	{
		SDL_ShowCursor(0);
	}
}