#ifndef GAME_H
#define GAME_H

#include "CoreEngine.h"

class Game
{
private:
	ProfileTimer m_inputProfileTimer;
	ProfileTimer m_updateProfileTimer;

	//void operator =(Game& game) {}

protected:
	GameObject rootObject;

	void addToScene(GameObject* object);
	void addToScene2(PhysicsObject* object);

public:
	Game() {};
	virtual ~Game() {};

	virtual void initialise(const Window& window);
	virtual void processInput(const Input& input, float delta);
	virtual void update(float delta);
	virtual void integrate(PhysicsEngine* physicsEngine, float delta);
	virtual void render(RenderingEngine* renderingEngine, const Camera& camera);

	double displayInputTime(double divisor);
	double displayUpdateTime(double divisor);

	GameObject& getRoot();

	void setEngine(CoreEngine* engine);
};

#endif