#include <SDL2/SDL.h>
#include <GL\glew.h>
#include "../Core/SDLWrapper.h"
#include "Window.h"

int Window::width = 0;
int Window::height = 0;
std::string Window::title = "Title";

void Window::create(int width, int height, const std::string& title)
{
	Window::width = width;
	Window::height = height;
	Window::title = title;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDLCreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, false);

	GLenum res = glewInit();

	if(res != GLEW_OK)
	{
		// Error
	}
}

void Window::render()
{
	SDLSwapBuffers();
}

void Window::dispose()
{
	SDLDestroyWindow();
	SDL_Quit();
}

void Window::bindAsRenderTarget()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, getWidth(), getHeight());
}

bool Window::isCloseRequested()
{
	return SDLGetIsCloseRequested();
}

int Window::getWidth()
{
	return width;
}

int Window::getHeight()
{
	return height;
}

float Window::getAspectRatio()
{
	return (float)width / (float)height;
}

std::string Window::getTitle()
{
	return title;
}

void Window::setFullscreen(bool value)
{
	SDLSetFullScreen(value);
}