#include <thread>

#include "CoreEngine.h"
#include "Game.h"
#include "Time.h"
#include "Util.h"

void test(float i, std::string j)
{
	printf("%f, %s\n", i, j.c_str());
}

CoreEngine::CoreEngine(int width, int height, double frameRate, Game* game)
{
	m_isRunning = false;
	m_width = width;
	m_height = height;
	m_frameRate = 1.0 / frameRate;
	m_game = game;
	m_renderingEngine = 0;
	m_physicsEngine = 0;

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

	//if(Lua::L)
	//{
	//	//lua_close(Lua::luaState);
	//}
}

void CoreEngine::createWindow(const std::string& title)
{
	m_window = new Window(m_width, m_height, title);
	m_renderingEngine = new RenderingEngine(*m_window);
	m_physicsEngine = new PhysicsEngine();
}

void CoreEngine::start()
{
	if(m_isRunning)
	{
		return;
	}

	run();
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

	m_game->initialise(*m_window);

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
			double totalTime = (1000.0 * frameCounter) / (double)frames;
			double totalRecordedTime = 0.0;

			totalRecordedTime += ProfileTimers::inputTimer__.displayAndReset("Input     - ", (double)frames);
			totalRecordedTime += ProfileTimers::updateTimer_.displayAndReset("Update    - ", (double)frames);
			totalRecordedTime += ProfileTimers::physicsTimer.displayAndReset("Physics   - ", (double)frames);
			totalRecordedTime += ProfileTimers::renderTimer_.displayAndReset("Render    - ", (double)frames);
			totalRecordedTime += ProfileTimers::wSyncTimer__.displayAndReset("WinSync   - ", (double)frames);
			totalRecordedTime += ProfileTimers::wUpdateTimer.displayAndReset("WinUpdate - ", (double)frames);
			totalRecordedTime += ProfileTimers::swapBufTimer.displayAndReset("Buff Swap - ", (double)frames);
			totalRecordedTime += ProfileTimers::sleepTimer__.displayAndReset("Sleep     - ", (double)frames);
			
			printf("Other     - %f\tms\n", totalTime - totalRecordedTime);
			printf("Total     - %f\tms\n\n", totalTime);
			
			frames = 0;
			frameCounter = 0;
		}

		while(unproccessedTime > m_frameRate)
		{
			render = true;

			ProfileTimers::wUpdateTimer.startInvocation();
			if(m_window->isCloseRequested())
			{
				stop();
			}

			m_window->update();
			ProfileTimers::wUpdateTimer.stopInvocation();

			m_game->processInput(m_window->getInput(), (float)m_frameRate);
			m_game->update((float)m_frameRate);

			std::thread physics_thread(&Game::integrate, m_game, m_physicsEngine, (float)m_frameRate);
			physics_thread.join();
			//m_game->integrate(m_physicsEngine, (float)m_frameRate);

			unproccessedTime -= m_frameRate;
		}

		if(render)
		{
			m_game->render(m_renderingEngine, *m_mainCamera);

			ProfileTimers::swapBufTimer.startInvocation();
			m_window->swapBuffers();
			ProfileTimers::swapBufTimer.stopInvocation();

			frames++;
		}
		else
		{
			ProfileTimers::sleepTimer__.startInvocation();
			Util::sleep(1);
			ProfileTimers::sleepTimer__.stopInvocation();
		}
	}
}

RenderingEngine* CoreEngine::getRenderingEngine()
{
	return m_renderingEngine;
}

PhysicsEngine* CoreEngine::getPhysicsEngine()
{
	return m_physicsEngine;
}

void CoreEngine::setCamera(const Camera& camera)
{
	m_mainCamera = &camera;
}