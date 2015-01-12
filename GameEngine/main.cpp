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

	int n = 20;
	int m = n / 2;

	for(int i = 0; i < n; i++)
	{
		int x = random(-m, m);
		int y = random(-m, m);
		int z = random(-m, m);
		int radius = 1;
		PhysicsObject* p = new PhysicsObject(new RigidBody(10), new BoundingSphere(radius), Vector3(x, y, z));
		//GameObject* p = new GameObject(Vector3(x, y, z));

		Material m = Material("", TEXTURE_BLANK, COLOUR_WHITE);
		p->addComponent(new MeshRenderer(Mesh("sphere.obj"), m));

		//p->velocity = Vector3(0, 0, 0);
		//p->r = 1;

		if(i == 0)
		{
			p->getTransform()->setPosition(Vector3(x + 10, y, z));
			p->addComponent(new Movement2D(10, Input::KEY_I, Input::KEY_K, Input::KEY_J, Input::KEY_L));
		}

		addToScene2(p);
	}

	addToScene(cameraObj);

	int partitions = Octree::partitions.size();

	for(int i = 0; i < partitions; i++)
	{
		GameObject* g = new GameObject(Octree::partitions[i].centre);
		AABB* a = new AABB(Octree::partitions[i].min, Octree::partitions[i].max);
		g->addComponent(a);
		addToScene(g);
	}

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