#include "Inneall.h"

#include <iostream>

#include "Engine\Components\Physics\PhysicsEngineComponent.h"
#include "Engine\Components\Physics\PhysicsObjectComponent.h"

#include "Engine\Rendering\SpriteSheet.h"
#include "Engine\Components\Rendering\SpriteRenderer.h"
#include "Engine\Components\Rendering\SpriteAnimator.h"

#include "Engine\Components\Game\Movement2D.h"

#include "Engine\Core\Script.h"
#include "Engine\Components\MoveScript.h"

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
	cameraObj->getTransform()->getPosition().setX(2);
	cameraObj->getTransform()->getPosition().setZ(0);
	cameraObj->addComponent(new FreeLook(window.getCentre()));
	//cameraObj->getTransform()->rotate(AXIS_Y, toRadians(180));
	cameraObj->addComponent(new FreeMove());
	cameraObj->addComponent(new CameraComponent(Matrix4().initPerspective(toRadians(90.0f), window.getAspectRatio(), 0.1f, 1000.0f)));

	addToScene(cameraObj);
	int y = 0;
	int x = 2;

	Colour c = COLOUR_AQUAMARINE;
	Vector4 v = c.getCMYK();

	GameObject* obj_ = new GameObject(Vector3(-4, y -1, 10));
	SpriteSheet s = SpriteSheet("", Material("", TEXTURE_BLANK, c), 1, 1);
	obj_->addComponent(new SpriteRenderer(s));
	obj_->addComponent(new Script("rotate.lua"));
	obj_->addComponent(new MoveScript(Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT, 0.1f));

	obj_->addComponent(new Movement2D());

	addToScene(obj_);

	for(int i = 0; i < 5; i++)
	{
		if(i >= 5)
		{
			y = -2;
		}

		GameObject* obj = new GameObject(Vector3(x * i, y, 10));
		SpriteSheet s = SpriteSheet("", Material("", TEXTURE_BLANK, getRandomColour(c)), 1, 1);
		obj->addComponent(new SpriteRenderer(s));
		addToScene(obj);
	}

	for(int i = 0; i < 5; i++)
	{
		if(i >= 5)
		{
			y = -2;
		}

		GameObject* obj = new GameObject(Vector3(x * i, y - 2, 10));
		SpriteSheet s = SpriteSheet("", Material("", TEXTURE_BLANK, getRandomColour(c)), 1, 1);
		obj->addComponent(new SpriteRenderer(s));
		addToScene(obj);
	}
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