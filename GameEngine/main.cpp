#include "Inneall.h"

#include <iostream>

#include "Engine\Rendering\SpriteSheet.h"
#include "Engine\Components\Rendering\SpriteRenderer.h"
#include "Engine\Components\Rendering\SpriteAnimator.h"

#include "Engine\Components\Game\Movement2D.h"

#include "Engine\Rendering\Shape.h"

#include "Engine\Core\Scripter.h"

#include "Engine\Components\Physics\ColliderRenderer.h"
#include "Engine\Components\Physics\PhysicsInput.h"

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

	Material mat = Material("mat", TEXTURE_BLANK, COLOUR_RED);
	//int i = 0;
	int s = 30;
	for(int i = 0; i < 1; i++)
	{
		for(int j = 0; j < 1; j++)
		{
			Vector3 v(-100, -100, -100);
			switch(j)
			{
			case 0:
				v = Vector3(0, s, 2.5f);
				break;
			case 1:
				v = Vector3(0, s, -2.5f);
				break;
			case 2:
				v = Vector3(2.5f, s, 0);
				break;
			case 3:
				v = Vector3(-2.5f, s, 0);
				break;
			}

			float f = random(1, 1);
			PhysicsComponent* physicsComponent = new PhysicsComponent(new ColliderBox(), new RigidBody(1));
			GameObject* cubeoObj = new GameObject(v);
			printf("i %d, j %d\n", i, j);
			//cubeoObj->addComponent(new MeshRenderer(Mesh("sphere.obj"), mat));
			cubeoObj->addComponent(physicsComponent);
			cubeoObj->addComponent(new PhysicsInput(*physicsComponent, Input::KEY_1));
			cubeoObj->addComponent(new ColliderRenderer(physicsComponent));
			addToScene(cubeoObj);
		}

		s += 5;
	}

	PhysicsComponent* planeComponent = new PhysicsComponent(new ColliderPlane(Vector3(0, 1, 0), -5));
	//PhysicsComponent* planeComponent = new PhysicsComponent(new ColliderBox(Vector3(5, 5, 5)), new RigidBody(1000000000));
	GameObject* planeObj = new GameObject(Vector3(0, -5, 0));
	//planeObj->getTransform()->setScale(2);
	planeObj->addComponent(planeComponent);
	planeObj->addComponent(new ColliderRenderer(planeComponent));

	addToScene(planeObj);

	Game::initialise(window);
}

void OctreeDemo::initialise(const Window& window)
{
	Game::initialise(window);
}

int main()
{
	CollisionDemo test;
	CoreEngine engine(800, 600, 999, &test);
	engine.createWindow("Game ENGINE");
	engine.start();

	return 0;
}