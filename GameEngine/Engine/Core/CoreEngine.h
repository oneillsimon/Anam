#ifndef COREENGINE_H
#define COREENGINE_H

#include <string>
#include "../Rendering/RenderingEngine.h"

class Game;

class CoreEngine
{
private:
	bool isRunning;
	int width;
	int height;
	double frameRate;
	Game* game;
	RenderingEngine* renderingEngine;

	void run();

	void operator =(const CoreEngine& coreEgine) {};

public:
	CoreEngine();
	CoreEngine(int width, int height, double frameRate, Game* game);
	~CoreEngine();

	void createWindow(std::string title);
	void start();
	void stop();

	RenderingEngine* getRenderingEngine();
};

#endif