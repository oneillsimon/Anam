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
	GameObject* cameraObj = new GameObject();
	cameraObj->getTransform()->getPosition().setX(0);
	cameraObj->getTransform()->getPosition().setZ(-2);
	cameraObj->addComponent(new FreeLook(window.getCentre()));
	//cameraObj->getTransform()->rotate(AXIS_Y, toRadians(180));
	cameraObj->addComponent(new FreeMove());
	cameraObj->addComponent(new CameraComponent(Matrix4().initPerspective(toRadians(70.0f), window.getAspectRatio(), 0.1f, 1000.0f)));
	cameraObj->getTransform()->getPosition().setY(0);
	//cameraObj->getTransform()->rotate(AXIS_X, toRadians(30));

	addToScene(cameraObj);

	////DirectionalLight* d = new DirectionalLight(COLOUR_WHITE, 1);
	////addToScene(new GameObject()->addComponent(new DirectionalLight(COLOUR_WHITE, 1.0f)));

	//Material* m = new Material("", TEXTURE_BLANK, Colour(0, 0, 0), 1.0f, 8.0f);
	//Mesh* mm = new Mesh("monkey3.obj");



	//GameObject* leftObj = new GameObject(Vector3(-4, 0, 10));
	//leftObj->getTransform()->setScale(0.8f);

	//cameraObj->getTransform()->setPosition(Vector3(-4, 0, 12));
	//cameraObj->getTransform()->rotate(AXIS_Y, toRadians(180));
	////leftObj->getTransform()->rotate(AXIS_Y, toRadians(180));
	//SpriteSheet s = SpriteSheet("", *m, 1, 1);
	////leftObj->addComponent(new SpriteRenderer(s));
	//leftObj->addComponent(new MeshRenderer(*mm, *m));
	//leftObj->enableScripting();
	////leftObj->addComponent(new Script("rotate.lua", leftObj->getScriptManager()));

	//addToScene(leftObj);

	//GameObject* rightObj = new GameObject(Vector3(4, 0, 10));
	//SpriteSheet s_ = SpriteSheet("", Material("", TEXTURE_BLANK, getRandomColour()), 1, 1);
	//rightObj->addComponent(new SpriteRenderer(s_));
	//rightObj->enableScripting();
	//rightObj->addComponent(new Script("move.lua", rightObj->getScriptManager()));
	//rightObj->addComponent(new Script("test.lua", rightObj->getScriptManager()));
	//rightObj->addComponent(new Script("rotateControl.lua", rightObj->getScriptManager()));
	//addToScene(rightObj);

	//GameObject* light = new GameObject(Vector3(-4, 0, -10));
	//light->addComponent(new DirectionalLight(COLOUR_WHITE, 0.4f));
	//light->getTransform()->rotate(AXIS_XZ, toRadians(-45));
	////cameraObj->addChild(light);
	//addToScene(light);

	DirectionalLight* whiteLight = new DirectionalLight(COLOUR_WHITE, 0.4f);
	//DirectionalLight* blueLight = new DirectionalLight(COLOUR_BLUE,	0.2f);
	//DirectionalLight* yellowLight = new DirectionalLight(COLOUR_YELLOW, 1.4f);
	//
	GameObject* whitelightObj = new GameObject(Vector3(0, 0, 0));
	whitelightObj->getTransform()->rotate(AXIS_X, toRadians(-45));
	whitelightObj->addComponent(whiteLight);
	////whitelightObj->getTransform()->rotate(AXIS_Y, toRadians(90));
	////whitelightObj->getTransform()->rotate(AXIS_X, toRadians(45));
	//GameObject* bluelightObj = new GameObject();
	//bluelightObj->getTransform()->rotate(AXIS_X, toRadians(-90));
	//bluelightObj->addComponent(blueLight);
	//GameObject* yellowlightObj = new GameObject();
	//yellowlightObj->addComponent(yellowLight);

	//Mesh terrain = Mesh("terrain02.obj");
	//MeshRenderer* mRenderer = new MeshRenderer(terrain, MATERIAL_DEFAULT);
	//
	//GameObject* terrainObj = new GameObject();
	//terrainObj->addComponent(mRenderer);
	//
	addToScene(whitelightObj);
	//addToScene(bluelightObj);
	//addToScene(yellowlightObj);
	//addToScene(terrainObj);

	GameObject* planeObj = new GameObject();
	planeObj->getTransform()->setScale(100);
	planeObj->addComponent(new MeshRenderer(Mesh("plane.obj"), MATERIAL_DEFAULT));

	GameObject* cube = new GameObject(Vector3(0, 2, 0), Quaternion(), 0.5f);
	cube->addComponent(new MeshRenderer(Mesh("cube.obj"), Material("", TEXTURE_BLANK, COLOUR_BLUE)));
	cube->addComponent(new FreeMove(10, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT));

	//Scripter* scripter = new Scripter({"", ""});
	//scripter->addScript("rotateControl.lua");
	//
	//cube->addComponent(scripter);
	//addToScene(cube);

	GameObject* cube1 = new GameObject(Vector3(5, 2, 0), Quaternion(), 0.5f);
	cube1->addComponent(new MeshRenderer(Mesh("cube.obj"), Material("", TEXTURE_BLANK, COLOUR_ORANGE)));
	cube1->addComponent(new FreeMove(10, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT));

	addToScene(cube1);
	cube1->addComponent(new Scripter({"rotateControl.lua", "g.lua"}));

	Game::initialise(window);
}

int main()
{
	TestGame test;
	CoreEngine engine(800, 600, 999, &test);
	engine.createWindow("Game ENGINE");
	engine.start();

	return 0;
}