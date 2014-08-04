#include <iostream>
#include "../Rendering/RenderingEngine.h"
#include "Game.h"

void Game::addToScene(GameObject* object)
{
	rootObject.addChild(object);
}

void Game::processInput(const Input& input, float delta)
{
	rootObject.processInputAll(input, delta);
}

void Game::update(float delta)
{
	rootObject.updateAll(delta);
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