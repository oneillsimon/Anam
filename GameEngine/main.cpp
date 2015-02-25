#include "Inneall.h"

#include <iostream>

#include "Engine\Components\Physics\PhysicsEngineComponent.h"
#include "Engine\Components\Physics\PhysicsObjectComponent.h"

#include "Engine\Rendering\SpriteSheet.h"
#include "Engine\Components\Rendering\SpriteRenderer.h"
#include "Engine\Components\Rendering\SpriteAnimator.h"

#include "Engine\Components\Game\Movement2D.h"

#include "Engine\Rendering\Shape.h"

#include "Engine\Core\Scripter.h"

#include "Engine\Components\Physics\OctreeRenderer.h"
#include "Engine\Components\Physics\ColliderRenderer.h"
#include "Engine\Components\Physics\RigidBodyComponent.h"
#include "Engine\Components\Physics\Box.h"

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

class OctreeDemo : public Game
{
public: virtual void initialise(const Window& window);
};

class CollisionDemo : public Game
{
public: virtual void initialise(const Window& window);
};

static void tests();

void CollisionDemo::initialise(const Window& window)
{
	GameObject* cameraObj = new GameObject(Vector3(0, 5, -7.5f));

	cameraObj->addComponent(new FreeLook(window.getCentre()));
	cameraObj->addComponent(new FreeMove());
	cameraObj->addComponent(new CameraComponent(Matrix4().initPerspective(toRadians(70.0f), window.getAspectRatio(), 0.1f, 1000.0f)));

	addToScene(cameraObj);
	int y = -2;
	int spacing = 4;

	for(int l = 0; l < 1; l++)
	{
		for(int i = 0; i < 2; i++)
		{
			Vector3 v;

			if(i == 0)
				v = Vector3(-1.5f,  y * 3, -1.5f);
			else if(i == 1)
				v = Vector3(1.5f, i - y * 3, 1.5f);
			else if(i == 2)
				v = Vector3(1.5f, y * 3, -1.5f);
			else
				v = Vector3(-1.5f, y * 3, 1.5f);

			PhysicsObject* pObj3 = new PhysicsObject(v);
			//Box * b = new Box(pObj3);
			//b->setState(0);
			pObj3->getTransform()->setScale(1.0f);
			//pObj3->setCollider(b, b->getBody());
			pObj3->setCollider(new ColliderBox(Vector3(1, 1, 1)), new RigidBody(1, 0.1f, 0.1f));
			//pObj3->getTransform()->rotate(AXIS_Z, toRadians(30));

			pObj3->addComponent(new ColliderRenderer(false, pObj3->getCollider(), COLOUR_FIREBRICK));
			pObj3->addComponent(new FreeMove(10, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT));
			pObj3->addComponent(new Movement2D(10, Input::KEY_O, Input::KEY_P, -1, -1));
			pObj3->addComponent(new RigidBodyComponent(pObj3));
			//pObj3->setMass(100);

			addToScene2(pObj3);
		}
		y += spacing;
	}

	for(int i = 0; i < 1; i++)
	{
		PhysicsObject* testObj = new PhysicsObject(Vector3(-1.5f, -5, -1.5f));
		//testObj->setCollider(new ColliderSphere(10));
		//testObj->setMass(10);
		//testObj->addComponent(new ColliderRenderer(false, testObj->getCollider(), COLOUR_GREEN));
		//addToScene2(testObj);
	}

	//new ColliderBox(Vector3(s, s, s))
	//new ColliderPlane(Vector3(0, 1, 0), -10)
	//new ColliderSphere(1)

	float s = 1;
	PhysicsObject* plane = new PhysicsObject(Vector3(0.0f, -10, 0));
	//plane->getTransform()->rotate(AXIS_Z, toRadians(-45));
	//plane->getTransform()->setScale(0.5f);
	float h = s / 2.0f;
	//plane->getCollider()->m_body->setMass(10000000000000000);
	plane->setCollider(new ColliderBox(Vector3(s, s, s)), new RigidBody(100000000000, 0.1f, 0.1f));
	//plane->getCollider()->m_body->m_hasInfiniteMass = true;
	//plane->setCollider(new ColliderBox(Vector3(s, s, s)));
	//plane->getCollider()->getBody()->setOrientation(Quaternion(AXIS_Z, toRadians(-45)));
	plane->addComponent(new ColliderRenderer(true, plane->getCollider(), COLOUR_GREEN));
	addToScene2(plane);

	getRoot().addComponent(new OctreeRenderer(getRoot().getEngine()->getPhysicsEngine(),getRoot().getEngine()->getPhysicsEngine()->m_tree, COLOUR_YELLOW_GREEN));

	Game::initialise(window);
}

void OctreeDemo::initialise(const Window& window)
{
	GameObject* cameraObj = new GameObject(Vector3(0, 0, -7.5f));

	cameraObj->addComponent(new FreeLook(window.getCentre()));
	cameraObj->addComponent(new FreeMove());
	cameraObj->addComponent(new CameraComponent(Matrix4().initPerspective(toRadians(70.0f), window.getAspectRatio(), 0.1f, 1000.0f)));

	int n = 12;
	int m = 12;

	for(int i = 0; i < n; i++)
	{
		float x = random(-m, m);
		float y = random(-m, m);
		float z = random(-m, m);

		PhysicsObject* o = new PhysicsObject(Vector3(x, y, z));
		//o->setCollider(new ColliderSphere());
		//o->setMass(10);
		if(i == 0)
		{
			o->addComponent(new FreeMove(10, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT));
			o->addComponent(new Movement2D(10, Input::KEY_O, Input::KEY_P, -1, -1));
			o->addComponent(new RigidBodyComponent(o));
		}
		o->addComponent(new MeshRenderer(Mesh("sphere.obj"), Material("", TEXTURE_BLANK, COLOUR_GREEN)));
		addToScene2(o);
	}

	DirectionalLight* d = new DirectionalLight(COLOUR_WHITE, 0.04f);
	GameObject* gg = new GameObject();
	gg->addComponent(d);
	addToScene(gg);
	getRoot().addComponent(new OctreeRenderer(getRoot().getEngine()->getPhysicsEngine(), getRoot().getEngine()->getPhysicsEngine()->m_tree, COLOUR_YELLOW_GREEN));

	addToScene(cameraObj);

	Game::initialise(window);
}

int main()
{
	//OctreeDemo test;
	CollisionDemo test;
	CoreEngine engine(800, 600, 999, &test);
	engine.createWindow("Game ENGINE");
	engine.start();

	return 0;
}