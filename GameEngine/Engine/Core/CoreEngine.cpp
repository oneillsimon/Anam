#include <stdio.h>
#include "Time.h"
#include "../Rendering/Window.h"
#include "Input.h"
#include "Util.h"
#include "Game.h"
#include "CoreEngine.h"
#include "../Rendering/RenderUtil.h"

CoreEngine::CoreEngine(int width, int height, double frameRate, Game* game)
{
	m_isRunning = false;
	m_width = width;
	m_height = height;
	m_frameRate = 1.0 / frameRate;
	m_game = game;
	m_renderingEngine = 0;

	m_game->setEngine(this);
}

CoreEngine::~CoreEngine()
{
	if(m_window)
	{
		delete m_window;
	}

	if(m_renderingEngine)
	{
		delete m_renderingEngine;
	}
}

void CoreEngine::createWindow(const std::string& title)
{
	m_window = new Window(m_width, m_height, title);
	m_renderingEngine = new RenderingEngine(*m_window);
}

void CoreEngine::start()
{
	if(m_isRunning)
	{
		return;
	}

	run2();
}

void CoreEngine::stop()
{
	if(!m_isRunning)
	{
		return;
	}

	m_isRunning = false;
}

void CoreEngine::run()
{
	m_isRunning = true;

	m_game->init(*m_window);

	double lastTime = Time::getTime();
	double unproccessedTime = 0;
	double frameCounter = 0;
	int frames = 0;

	while(m_isRunning)
	{
		bool render = false;

		double startTime = Time::getTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unproccessedTime += passedTime;
		frameCounter += passedTime;

		if(frameCounter > 1.0)
		{
			printf("%d\n", frames);
			frames = 0;
			frameCounter = 0;
		}

		while(unproccessedTime > m_frameRate)
		{
			render = true;

			if(m_window->isCloseRequested())
			{
				stop();
			}

			m_window->update();

			m_game->processInput(m_window->getInput(), (float)m_frameRate);
			m_game->update((float)m_frameRate);

			unproccessedTime -= m_frameRate;
		}

		if(render)
		{
			m_game->render(m_renderingEngine, *m_mainCamera);
			m_window->swapBuffers();
			frames++;
		}
		else
		{
			Util::sleep(0);
		}
	}
}

void CoreEngine::run2()
{
	m_isRunning = true;

	m_game->init(*m_window);

	double lastTime = Time::getTime();
	double unprocessedTime = 0;
	double frameCounter = 0;
	int frames = 0;

	while(m_isRunning)
	{
		bool render = false;

		double startTime = Time::getTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		if(frameCounter >= 1.0)
		{
			//printf("%i\n",frames);
			printf("%f ms\n",1000.0/((double)frames));
			frames = 0;
			frameCounter = 0;
		}

		while(unprocessedTime > m_frameRate)
		{
			render = true;

			if(m_window->isCloseRequested())
			{
				stop();
			}

			m_window->update();

			m_game->processInput(m_window->getInput(), (float)m_frameRate);
			m_game->update((float)m_frameRate);

			unprocessedTime -= m_frameRate;
		}

		if(render)
		{
			m_game->render(m_renderingEngine, *m_mainCamera);
			m_window->swapBuffers();
			frames++;
		}
		else
		{
			Util::sleep(1);
		}
	}
}

RenderingEngine* CoreEngine::getRenderingEngine()
{
	return m_renderingEngine;
}

void CoreEngine::setCamera(const Camera& camera)
{
	m_mainCamera = &camera;
}