#include "Inneall.h"

#include <iostream>

#include "Engine\Components\Physics\PhysicsEngineComponent.h"
#include "Engine\Components\Physics\PhysicsObjectComponent.h"

#include "Engine\Rendering\SpriteSheet.h"
#include "Engine\Components\Rendering\SpriteRenderer.h"
#include "Engine\Components\Rendering\SpriteAnimator.h"

#include "Engine\Components\Game\Movement2D.h"

#include "Engine\Physics\Particle.h"
#include "Engine\Physics\PFGen.h"
#include "Engine\Physics\CollisionFine.h"
#include "Engine\Rendering\Shape.h"

#include "Engine\Core\Scripter.h"

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

	PhysicsObject* pObj2 = new PhysicsObject(new RigidBody(00.001f), new ColliderBox(2), Vector3(3, 1, -1));
	pObj2->addComponent(new MeshRenderer(Mesh("cube.obj"), Material("", TEXTURE_BLANK, COLOUR_MEDIUM_PURPLE)));
	pObj2->addComponent(new FreeMove(10, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT));
	pObj2->addComponent(new Movement2D(10, Input::KEY_O, Input::KEY_P, -1, -1));
	addToScene2(pObj2);

	PhysicsObject* plane = new PhysicsObject(new RigidBody(-1), new ColliderPlane(AXIS_Y, 0), Vector3(0, -12, 0));
	plane->getTransform()->setScale(Vector3(1000, 1, 1000));
	plane->addComponent(new MeshRenderer(Mesh("plane3.obj"), Material("", TEXTURE_BLANK, COLOUR_ORANGE)));
	addToScene2(plane);

	int n = 10;
	int m = 12;

	for(int i = 0; i < n; i++)
	{
		float x = random(-m, m);
		float y = random(-m, m);
		float z = random(-m, m);

		PhysicsObject* o = new PhysicsObject(new RigidBody(1), new ColliderSphere(), Vector3(x, y, z));
		o->addComponent(new MeshRenderer(Mesh("sphere.obj"), Material("", TEXTURE_BLANK, getRandomColour())));
		addToScene2(o);
	}

	DirectionalLight* d = new DirectionalLight(COLOUR_WHITE, 0.04f);
	GameObject* gg = new GameObject();
	gg->addComponent(d);
	addToScene(gg);

	addToScene(cameraObj);

	int pCount = Octree::partitions.size();
	
	for(int i = 0; i < pCount; i++)
	{
		GameObject* g = new GameObject(Octree::partitions[i].centre, Quaternion(), Octree::partitions[i].max.getX());
		g->addComponent(new ColliderRenderer(new ColliderBox(Octree::partitions[i].min)));
		addToScene(g);
	}

	//getRoot().addComponent(new OctreeRenderer(0));

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