#ifndef GAME_H
#define GAME_H

#include "GameObject.h"
#include "CoreEngine.h"

class Game
{
private:
	void operator =(Game& game) {}

protected:
	GameObject rootObject;
	void addToScene(GameObject* object);

public:
	Game();
	~Game();

	virtual void init();
	virtual void input(float delta);
	virtual void update(float delta);
	virtual void render(RenderingEngine* renderingEngine);

	GameObject& getRoot();

	void setEngine(CoreEngine* engine);
};

#endif