#include "Inneall.h"

#include <iostream>

#include "Engine\Components\Physics\PhysicsEngineComponent.h"
#include "Engine\Components\Physics\PhysicsObjectComponent.h"

#include "Engine\Rendering\SpriteSheet.h"
#include "Engine\Components\Rendering\SpriteRenderer.h"
#include "Engine\Components\Rendering\SpriteAnimator.h"

#include "Engine\Components\Game\Movement2D.h"

#include "Engine\Physics_\Particle.h"
#include "Engine\Physics_\Physics_Component.h"
#include "Engine\Physics_\PFGen.h"
#include "Engine\Physics_\CollideCoarse.h"
#include "Engine\Rendering\Shape.h"
#include "Engine\Physics\PhysicsComponent.h"

#include "Engine\Core\Scripter.h"

#include "Engine\Physics\Octree\Octree.h"
#include "Engine\Components\Physics\ColliderRenderer.h"

#undef main

class TestGame : public Game
{
public:
	TestGame(){};
	
	virtual void initialise(const Window& window);
protected:
private:
	//TestGame(const TestGame& other) {}
	//void operator =(const TestGame& other) {}
};

static void tests();

void TestGame::initialise(const Window& window)
{
	GameObject* cameraObj = new GameObject(Vector3(0, 0, -4));

	cameraObj->addComponent(new FreeLook(window.getCentre()));
	cameraObj->addComponent(new FreeMove());
	cameraObj->addComponent(new CameraComponent(Matrix4().initPerspective(toRadians(70.0f), window.getAspectRatio(), 0.1f, 1000.0f)));

	//PhysicsObject* pObj = new PhysicsObject(new RigidBody(10));
	PhysicsObject* pObj = new PhysicsObject(new RigidBody(10), Vector3(-0.5f, 0, 0));
	pObj->addComponent(new MeshRenderer(Mesh("sphere.obj"), MATERIAL_DEFAULT));
	addToScene2(pObj);

	PhysicsObject* pObj2 = new PhysicsObject(new RigidBody(10), Vector3(0.5f, 0, 0));
	pObj2->addComponent(new MeshRenderer(Mesh("sphere.obj"), MATERIAL_DEFAULT));
	pObj2->addComponent(new Movement2D(10));
	addToScene2(pObj2);

	addToScene(cameraObj);

	Game::initialise(window);
}

int main()
{
	TestGame test;
	CoreEngine engine(800, 600, 120, &test);
	engine.createWindow("Game ENGINE");
	engine.start();

	return 0;
}