#include "CoreEngine.h"
#include "Game.h"
#include "Time.h"
#include "Util.h"

CoreEngine::CoreEngine(int width, int height, double frameRate, Game* game)
{
	m_isRunning = false;
	m_width = width;
	m_height = height;
	m_frameRate = 1.0 / frameRate;
	m_game = game;
	m_renderingEngine = 0;
	m_physicsEngine = 0;

	luaL_openlibs(Lua::luaState);
	//luaopen_my_module(Lua::luaState);
	Lua::register_();

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

	if(Lua::luaState)
	{
		//lua_close(Lua::luaState);
	}
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
			double totalTime = (1000.0 * frameCounter) / (double)frames;
			double totalRecordedTime = 0.0;

			totalRecordedTime += m_game->displayInputTime((double)frames);
			totalRecordedTime += m_game->displayUpdateTime((double)frames);
			totalRecordedTime += m_renderingEngine->displayRenderTime((double)frames);
			totalRecordedTime += m_renderingEngine->displayWindowSyncTime((double)frames);
			totalRecordedTime += m_windowUpdateTimer.displayAndReset("Window Update Time: ", (double)frames);
			totalRecordedTime += m_swapBufferTimer.displayAndReset("Buffer Swap Time: ", (double)frames);
			totalRecordedTime += m_sleepTimer.displayAndReset("Sleep Time: ", (double)frames);
			
			printf("Other time: %f ms\n", totalTime - totalRecordedTime);
			printf("Total time: %f ms\n\n", totalTime);
			
			frames = 0;
			frameCounter = 0;
		}

		while(unproccessedTime > m_frameRate)
		{
			render = true;

			m_windowUpdateTimer.startInvocation();
			if(m_window->isCloseRequested())
			{
				stop();
			}

			m_window->update();
			m_windowUpdateTimer.stopInvocation();

			m_game->processInput(m_window->getInput(), (float)m_frameRate);
			m_game->update((float)m_frameRate);

			m_game->integrate(m_physicsEngine, (float)m_frameRate);

			unproccessedTime -= m_frameRate;
		}

		if(render)
		{
			m_game->render(m_renderingEngine, *m_mainCamera);

			m_swapBufferTimer.startInvocation();
			m_window->swapBuffers();
			m_swapBufferTimer.stopInvocation();

			frames++;
		}
		else
		{
			m_sleepTimer.startInvocation();
			Util::sleep(1);
			m_sleepTimer.stopInvocation();
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