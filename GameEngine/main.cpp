#include "Inneall.h"

#include <iostream>

#include "Engine\Rendering\SpriteSheet.h"
#include "Engine\Components\Rendering\SpriteRenderer.h"
#include "Engine\Components\Rendering\SpriteAnimator.h"

#include "Engine\Components\Game\Movement2D.h"

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

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			Collider* c;
			std::string s = "";
			int r = random(0, 1);

			if(r)
			{
				c = new ColliderBox();
				s = "cube.obj";
			}
			else
			{
				c = new ColliderSphere();
				s = "sphere.obj";
			}

			PhysicsComponent* physicsComponent = new PhysicsComponent(c, new RigidBody(1));
			GameObject* cubeoObj = new GameObject(Vector3(3 + (3 * i + (0.05f * j)), 3 + (3 * j), 0.05f * j));
			cubeoObj->addComponent(new MeshRenderer(Mesh(s), mat));
			cubeoObj->addComponent(physicsComponent);
			cubeoObj->addComponent(new ColliderRenderer(physicsComponent));
			addToScene(cubeoObj);
		}
	}

	PhysicsComponent* planeComponent = new PhysicsComponent(new ColliderPlane());
	
	GameObject* planeObj = new GameObject();
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