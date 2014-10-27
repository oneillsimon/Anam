#include "Inneall.h"

#include <iostream>

#include "Engine\Components\Physics\PhysicsEngineComponent.h"
#include "Engine\Components\Physics\PhysicsObjectComponent.h"

#include "Engine\Rendering\SpriteSheet.h"
#include "Engine\Components\Rendering\SpriteRenderer.h"
#include "Engine\Components\Rendering\SpriteAnimator.h"

#include "Engine\Components\TestComponent.h"

#include "Engine\Core\Script.h"

#include "Engine\Physics_\Particle.h"
#include "Engine\Physics_\Physics_Component.h"
#include "Engine\Physics_\PFGen.h"
#include "Engine\Physics_\CollideCoarse.h"
#include "Engine\Rendering\Shape.h"
#include "Engine\Physics\PhysicsComponent.h"

#undef main

class TestGame : public Game
{
public:
	TestGame(){};
	
	virtual void init(const Window& window);
protected:
private:
	//TestGame(const TestGame& other) {}
	//void operator =(const TestGame& other) {}
};

static void tests();

void TestGame::init(const Window& window)
{
	MeshRenderer* terrainRenderer = new MeshRenderer(Mesh("terrain02.obj"), MATERIAL_DEFAULT);
	GameObject* terrainObj = new GameObject();
	terrainObj->getTransform()->setPosition(Vector3(-20, -20, 20));
	terrainObj->addComponent(terrainRenderer);

	MeshRenderer* planeRenderer = new MeshRenderer(Mesh("plane.obj"), MATERIAL_DEFAULT);
	GameObject* planeObj = new GameObject();
	planeObj->getTransform()->setPosition(Vector3(10, -10, 20));
	planeObj->addComponent(planeRenderer);


	GameObject* cameraObj = new GameObject();
	cameraObj->addComponent(new FreeLook(window.getCentre()));
	//cameraObj->getTransform()->rotate(AXIS_Y, toRadians(180));
	cameraObj->addComponent(new FreeMove());
	cameraObj->addComponent(new CameraComponent(Matrix4().initPerspective(toRadians(70.0f), window.getAspectRatio(), 0.1f, 1000.0f)));

	GameObject* dirLightObj = new GameObject(Vector3(0, 10, 0), Quaternion(), 2);
	dirLightObj->getTransform()->rotate(Quaternion());
	dirLightObj->addComponent(new DirectionalLight(COLOUR_WHITE, 0.4f, 0, 80.0f, 1.0f));

	terrainObj->addComponent(new Script("test.lua", 0.1f));
	planeObj->addComponent(new Script("test.lua", 0.2f));

	addToScene(cameraObj);
	addToScene(dirLightObj);
	addToScene(terrainObj);
	addToScene(planeObj);
}

int main()
{
	//tests();
	//getchar();

	TestGame test;
	CoreEngine engine(800, 600, 60, &test);
	engine.createWindow("Game ENGINE");
	engine.start();

	return 0;
}