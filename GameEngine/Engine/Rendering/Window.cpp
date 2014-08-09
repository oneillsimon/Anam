#include <SDL2/SDL.h>
#include <GL\glew.h>
#include "../Core/SDLWrapper.h"
#include "../Core/Profiling.h"
#include "Window.h"

Window::Window(int width, int height, const std::string& title)
{
	m_width = width;
	m_height = height;
	m_title = title;
	m_input = this;
	m_isCloseRequested = false;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_window);

	SDL_GL_SetSwapInterval(1);

	// For xcode
	glewExperimental = true;

	GLenum res = glewInit();

	if(res != GLEW_OK)
	{
		fprintf(stderr, "Error int glewInit()");
	}
}

Window::~Window()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Window::update()
{
	for(int i = 0; i < NUM_MOUSE_BUTTONS; i++)
	{
		m_input.setMouseDown(i, false);
		m_input.setMouseUp(i, false);
	}

	for(int i = 0; i < NUM_KEYS; i++)
	{
		m_input.setKeyDown(i, false);
		m_input.setKeyUp(i, false);
	}

	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		if(e.type == SDL_QUIT)
		{
			m_isCloseRequested = true;
		}

		if(e.type == SDL_MOUSEMOTION)
		{
			m_input.setMouseX(e.motion.x);
			m_input.setMouseY(e.motion.y);
		}

		if(e.type == SDL_KEYDOWN)
		{
			int value = e.key.keysym.scancode;
			m_input.setKey(value, true);
			m_input.setKeyDown(value, true);
		}

		if(e.type == SDL_KEYUP)
		{
			int value = e.key.keysym.scancode;
			m_input.setKey(value, false);
			m_input.setKeyUp(value, true);
		}

		if(e.type == SDL_MOUSEBUTTONDOWN)
		{
			int value = e.button.button;
			m_input.setMouse(value, true);
			m_input.setMouseDown(value, true);
		}

		if(e.type == SDL_MOUSEBUTTONUP)
		{
			int value = e.button.button;
			m_input.setMouse(value, false);
			m_input.setMouseUp(value, true);
		}
	}
}

void Window::swapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

void Window::bindAsRenderTarget() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#if PROFILING_SET_1x1_VIEWPORT == 0
	glViewport(0, 0, getWidth(), getHeight());
#else
	glViewport(0, 0, 1, 1);
#endif
}

void Window::setFullScreen(bool b)
{
	int mode = 0;

	if(b)
	{
		mode = SDL_WINDOW_FULLSCREEN;
	}
	else
	{
		mode = 0;
	}

	SDL_SetWindowFullscreen(m_window, mode);
}

int Window::getWidth() const
{
	return m_width;
}

int Window::getHeight() const
{
	return m_height;
}

const std::string& Window::getTitle() const
{
	return m_title;
}

float Window::getAspectRatio() const
{
	return (float)m_width / (float)m_height;
}

Vector2 Window::getCentre() const
{
	return Vector2((float)m_width / 2.0f, (float)m_height / 2.0f);
}

SDL_Window* Window::getWindow()
{
	return m_window;
}

const Input& Window::getInput() const
{
	return m_input;
}

bool Window::isCloseRequested() const
{
	return m_isCloseRequested;
}