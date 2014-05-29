#include <SDL2/SDL.h>
#include <iostream>
#include "SDLWrapper.h"

static SDL_Window* window;
static SDL_GLContext glContext;
static bool isCloseRequested = false;

bool SDLGetIsCloseRequested()
{
	return isCloseRequested;
}

void SDLSetIsCloseRequested(bool value)
{
	isCloseRequested = value;
}

void SDLCreateWindow(const char* title, int x, int y, int width, int height, bool fullScreen)
{
	int mode = 0;

	if(fullScreen)
	{
		mode = SDL_WINDOW_FULLSCREEN;
	}

	window = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_OPENGL | mode);
	glContext = SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(1);
}

void SDLSetFullScreen(bool value)
{
	int mode = 0;

	if(value)
	{
		mode = SDL_WINDOW_FULLSCREEN;
	}
	
	SDL_SetWindowFullscreen(window, mode);
}

void SDLSwapBuffers()
{
	SDL_GL_SwapWindow(window);
}

void SDLDestroyWindow()
{
	if(!window)
	{
		return;
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
}

void SDLSetMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(window, x, y);
}