#ifndef WINDOW_H
#define WINDOW_H

#include <string>

class Window
{
private:
	static int width;
	static int height;
	static std::string title;

public:
	static void create(int width, int height, const std::string& title);
	static void render();
	static void dispose();
	static void bindAsRenderTarget();

	static bool isCloseRequested();

	static int getWidth();
	static int getHeight();
	static float getAspectRatio();
	static std::string getTitle();

	static void setFullscreen(bool value);
};

#endif