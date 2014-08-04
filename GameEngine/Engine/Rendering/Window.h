#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <SDL2/SDL.h>
#include "../Core/Input.h"

class Window
{
private:
	int m_width;
	int m_height;
	std::string m_title;
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	Input m_input;
	bool m_isCloseRequested;

	Window(const Window& other) : m_input(this) {}
	void operator =(const Window& other) {}

public:
	Window(int width, int height, const std::string& title);
	virtual ~Window();

	void update();
	void swapBuffers();
	void bindAsRenderTarget() const;
	void setFullScreen(bool b);

	int getWidth() const;
	int getHeight() const;
	const std::string& getTitle() const;
	float getAspectRatio() const;
	Vector2 getCentre() const;
	SDL_Window* getWindow();
	const Input& getInput() const;
	bool isCloseRequested() const;
};

#endif