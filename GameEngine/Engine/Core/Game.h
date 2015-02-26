#ifndef GAME_H
#define GAME_H

#include "CoreEngine.h"

class Game
{
private:
	//void operator =(Game& game) {}

protected:
	GameObject rootObject;

	void addToScene(GameObject* object);

public:
	Game() {};
	virtual ~Game() {};

	virtual void initialise(const Window& window);
	virtual void processInput(const Input& input, float delta);
	virtual void update(float delta);
	virtual void integrate(PhysicsEngine* physicsEngine, float delta);
	virtual void render(RenderingEngine* renderingEngine, const Camera& camera);

	GameObject& getRoot();

	void setEngine(CoreEngine* engine);
};

#endif