#include "Inneall.h"

#include <iostream>

#include "Engine\Components\Physics\PhysicsEngineComponent.h"
#include "Engine\Components\Physics\PhysicsObjectComponent.h"

#include "Engine\Rendering\SpriteSheet.h"
#include "Engine\Components\Rendering\SpriteRenderer.h"
#include "Engine\Components\Rendering\SpriteAnimator.h"

#include "Engine\Components\Game\Movement2D.h"

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
	GameObject* cameraObj = new GameObject();
	cameraObj->getTransform()->getPosition().setX(0);
	cameraObj->getTransform()->getPosition().setZ(0);
	cameraObj->addComponent(new FreeLook(window.getCentre()));
	//cameraObj->getTransform()->rotate(AXIS_Y, toRadians(180));
	cameraObj->addComponent(new FreeMove());
	cameraObj->addComponent(new CameraComponent(Matrix4().initPerspective(toRadians(70.0f), window.getAspectRatio(), 0.1f, 1000.0f)));

	addToScene(cameraObj);

	GameObject* leftObj = new GameObject(Vector3(-4, 0, 10));
	SpriteSheet s = SpriteSheet("", Material("", TEXTURE_BLANK, getRandomColour()), 1, 1);
	leftObj->addComponent(new SpriteRenderer(s));
	leftObj->enableScripting();
	leftObj->addComponent(new Script("test2.lua", leftObj->getScriptManager()));
	//leftObj->addComponent(new Script("rotate.lua", leftObj->getScriptManager()));
	//leftObj->addComponent(new Script("move.lua", leftObj->getScriptManager()));

	addToScene(leftObj);

	GameObject* rightObj = new GameObject(Vector3(4, 0, 10));
	SpriteSheet s_ = SpriteSheet("", Material("", TEXTURE_BLANK, getRandomColour()), 1, 1);
	rightObj->addComponent(new SpriteRenderer(s_));
	rightObj->enableScripting();
	//rightObj->addComponent(new Script("move.lua", rightObj->getScriptManager()));
	//rightObj->addComponent(new Script("test.lua", rightObj->getScriptManager()));
	//rightObj->addComponent(new Script("rotateControl.lua", rightObj->getScriptManager()));
	addToScene(rightObj);
}

int main()
{
	TestGame test;
	CoreEngine engine(800, 600, 120, &test);
	engine.createWindow("Game ENGINE");
	engine.start();

	return 0;
}