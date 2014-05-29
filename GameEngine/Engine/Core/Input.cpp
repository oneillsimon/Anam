#include <SDL2/SDL.h>
#include "SDLWrapper.h"
#include "Input.h"

const static int NUM_KEYS = 512;
const static int NUM_MOUSE_BUTTONS = 256;

static SDL_Event e;

static int mouseX = 0;
static int mouseY = 0;

static bool inputs[NUM_KEYS];
static bool downKeys[NUM_KEYS];
static bool upKeys[NUM_KEYS];

static bool mouseInput[NUM_MOUSE_BUTTONS];
static bool downMouse[NUM_MOUSE_BUTTONS];
static bool upMouse[NUM_MOUSE_BUTTONS];

void Input::update()
{
	for(int i = 0; i < NUM_MOUSE_BUTTONS; i++)
	{
		downMouse[i] = false;
		upMouse[i] = false;
	}

	for(int i = 0; i < NUM_KEYS; i++)
	{
		downKeys[i] = false;
		upKeys[i] = false;
	}

	while(SDL_PollEvent(&e))
	{
		if(e.type == SDL_QUIT)
		{
			SDLSetIsCloseRequested(true);
		}

		if(e.type == SDL_MOUSEMOTION)
		{
			mouseX = e.motion.x;
			mouseY = e.motion.y;
		}

		if(e.type == SDL_KEYDOWN)
		{
			int value = e.key.keysym.scancode;

			inputs[value] = true;
			downKeys[value] = true;
		}

		if(e.type == SDL_KEYUP)
		{
			int value = e.key.keysym.scancode;

			inputs[value] = false;
			upKeys[value] = true;
		}

		if(e.type == SDL_MOUSEBUTTONDOWN)
		{
			int value = e.button.button;

			mouseInput[value] = true;
			downMouse[value] = true;
		}

		if(e.type == SDL_MOUSEBUTTONUP)
		{
			int value = e.button.button;

			mouseInput[value] = false;
			upMouse[value] = true;
		}
	}
}

bool Input::getKey(int keyCode)
{
	return inputs[keyCode];
}

bool Input::getKeyDown(int keyCode)
{
	return downKeys[keyCode];
}

bool Input::getKeyUp(int keyCode)
{
	return upKeys[keyCode];
}

bool Input::getMouse(int button)
{
	return mouseInput[button];
}

bool Input::getMouseDown(int button)
{
	return downMouse[button];
}

bool Input::getMouseUp(int button)
{
	return upMouse[button];
}

Vector2 Input::getMousePosition()
{
	return Vector2((float)mouseX, (float)mouseY);
}

void Input::setMousePosition(Vector2 position)
{
	SDLSetMousePosition((int)position.getX(), (int)position.getY());
}

void Input::setCursor(bool value)
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