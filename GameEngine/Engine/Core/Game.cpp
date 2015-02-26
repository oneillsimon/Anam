#include <thread>

#include "Game.h"

void Game::initialise(const Window& window)
{
	rootObject.initialiseAll();
}

void Game::addToScene(GameObject* object)
{
	rootObject.addChild(object);
}

void Game::processInput(const Input& input, float delta)
{
	ProfileTimers::inputTimer__.startInvocation();
	rootObject.inputAll(input, delta);
	ProfileTimers::inputTimer__.stopInvocation();
}

void Game::update(float delta)
{
	ProfileTimers::updateTimer_.startInvocation();
	rootObject.updateAll(delta);
	ProfileTimers::updateTimer_.stopInvocation();
}

void Game::integrate(PhysicsEngine* physicsEngine, float delta)
{
	physicsEngine->simulate(delta);
}

void Game::render(RenderingEngine* renderingEngine, const Camera& camera)
{
	renderingEngine->render(rootObject, camera);
}

GameObject& Game::getRoot()
{
	return rootObject;
}

void Game::setEngine(CoreEngine* coreEngine)
{
	rootObject.setEngine(coreEngine);
}