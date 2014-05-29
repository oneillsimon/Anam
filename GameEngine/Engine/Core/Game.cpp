#include <iostream>
#include "../Rendering/RenderingEngine.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::init()
{
}

void Game::addToScene(GameObject* object)
{
	rootObject.addChild(object);
}

void Game::input(float delta)
{
	rootObject.inputAll(delta);
}

void Game::update(float delta)
{
	rootObject.updateAll(delta);
}

void Game::render(RenderingEngine* renderingEngine)
{
	renderingEngine->render(&rootObject);
}

GameObject& Game::getRoot()
{
	return rootObject;
}

void Game::setEngine(CoreEngine* coreEngine)
{
	rootObject.setEngine(coreEngine);
}