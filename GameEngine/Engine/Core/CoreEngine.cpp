#include <stdio.h>
#include "Time.h"
#include "../Rendering/Window.h"
#include "Input.h"
#include "../Rendering/RenderUtil.h"
#include "Util.h"
#include "Game.h"
#include "CoreEngine.h"

CoreEngine::CoreEngine(int width, int height, double frameRate, Game* game) :
	isRunning(false),
	width(width),
	height(height),
	frameRate(1.0 / frameRate),
	game(game),
	renderingEngine(NULL)
{
	this->game->setEngine(this);
}

CoreEngine::~CoreEngine()
{
	Window::dispose();

	if(renderingEngine)
	{
		delete renderingEngine;
	}
}

void CoreEngine::createWindow(std::string title)
{
	Window::create(width, height, title);
	RenderUtil::initGraphics();
	this->renderingEngine = new RenderingEngine();
}

void CoreEngine::start()
{
	if(isRunning)
	{
		return;
	}

	run();
}

void CoreEngine::stop()
{
	if(!isRunning)
	{
		return;
	}

	isRunning = false;
}

void CoreEngine::run()
{
	isRunning = true;

	game->init();

	double lastTime = Time::getTime();
	double unproccessedTime = 0;
	double frameCounter = 0;
	int frames = 0;

	while(isRunning)
	{
		bool render = false;

		double startTime = Time::getTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unproccessedTime += passedTime;
		frameCounter += passedTime;

		if(frameCounter > 2.0)
		{
			printf("%d\n", frames);
			frames = 0;
			frameCounter = 0;
		}

		while(unproccessedTime > frameRate)
		{
			render = true;

			if(Window::isCloseRequested())
			{
				stop();
			}

			Input::update();

			game->input((float)frameRate);
			game->update((float)frameRate);

			unproccessedTime -= frameRate;
		}

		if(render)
		{
			game->render(renderingEngine);
			Window::render();
			frames++;
		}
		else
		{
			Util::sleep(0);
		}
	}
}

RenderingEngine* CoreEngine::getRenderingEngine()
{
	return renderingEngine;
}