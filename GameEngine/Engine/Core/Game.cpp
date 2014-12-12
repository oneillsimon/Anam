#include "Game.h"

void Game::initialise(const Window& window)
{
	rootObject.initialiseAll();
}

void Game::addToScene(GameObject* object)
{
	rootObject.addChild(object);
}

void Game::addToScene2(PhysicsObject* object)
{
	rootObject.addChild(object);
	rootObject.getEngine()->getPhysicsEngine()->addObject(object);
}

void Game::processInput(const Input& input, float delta)
{
	m_inputProfileTimer.startInvocation();
	rootObject.inputAll(input, delta);
	m_inputProfileTimer.stopInvocation();
}

void Game::update(float delta)
{
	m_updateProfileTimer.startInvocation();
	rootObject.updateAll(delta);
	m_updateProfileTimer.stopInvocation();
}

void Game::integrate(PhysicsEngine* physicsEngine, float delta)
{
	physicsEngine->simulate(rootPhysicsObject, delta);
}

void Game::render(RenderingEngine* renderingEngine, const Camera& camera)
{
	renderingEngine->render(rootObject, camera);
}

double Game::displayInputTime(double divisor)
{
	return m_inputProfileTimer.displayAndReset("Input Time: ", divisor);
}

double Game::displayUpdateTime(double divisor)
{
	return m_updateProfileTimer.displayAndReset("Update Time: ", divisor);
}

GameObject& Game::getRoot()
{
	return rootObject;
}

void Game::setEngine(CoreEngine* coreEngine)
{
	rootObject.setEngine(coreEngine);
	rootPhysicsObject.setEngine(coreEngine);
}