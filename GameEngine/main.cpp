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
	Material bricks("bricks", Texture("bricks.jpg"), COLOUR_ALICE_BLUE, 0.5f, 4, Texture("bricks_normal.jpg"), Texture("bricks_disp.png"), 0.03f, -0.5f);
	Material bricks_("bricks_", Texture("bricks.jpg"), COLOUR_ALICE_BLUE, 0.5f, 4, Texture("bricks_normal.jpg"), Texture("bricks_disp.png"), 0.03f, -0.5f);
	Material bricks2("bricks2", Texture("bricks2.jpg"), COLOUR_WHITE, 1, 8, Texture("bricks2_normal.jpg"), Texture("bricks2_disp.jpg"), 0.04f, -1.0f);
	Material blank("blank", Texture("white.png"));
	Material default("default", Texture(""));
	Material braid("braid", Texture("braid-dinosingle.png"));
	MeshRenderer* terrainRenderer = new MeshRenderer(Mesh("terrain02.obj"), blank);
	GameObject* terrainObj = new GameObject();
	terrainObj->addComponent(terrainRenderer);

	//IndexedModel cube;
	//{
	//	//cube.addVertex(1.0f, -1.0f, 0.0f);  cube.addTexCoord(Vector2(1.0f, 1.0f));
	//	//cube.addVertex(1.0f, 1.0f, 0.0f);   cube.addTexCoord(Vector2(1.0f, 0.0f));
	//	//cube.addVertex(-1.0f, -1.0f, 0.0f); cube.addTexCoord(Vector2(0.0f, 1.0f));
	//	//cube.addVertex(-1.0f, 1.0f, 0.0f);  cube.addTexCoord(Vector2(0.0f, 0.0f));
	//	cube.addVertex(1.0f, -1.0f, 0.0f);  cube.addTexCoord(Vector2(1.0f / 3.0f, 1.0f / 3.0f));
	//	cube.addVertex(1.0f, 1.0f, 0.0f);   cube.addTexCoord(Vector2(1.0f / 3.0f, 0.0f / 3.0f));
	//	cube.addVertex(-1.0f, -1.0f, 0.0f); cube.addTexCoord(Vector2(0.0f / 3.0f, 1.0f / 3.0f));
	//	cube.addVertex(-1.0f, 1.0f, 0.0f);  cube.addTexCoord(Vector2(0.0f / 3.0f, 0.0f / 3.0f));
	//	cube.addFace(0, 1, 2); cube.addFace(2, 1, 3);
	//}
	//Mesh customMesh("cube", cube.finalize());

	GameObject* cameraObj = new GameObject();
	cameraObj->addComponent(new FreeLook(window.getCentre()));
	cameraObj->addComponent(new FreeMove());
	cameraObj->addComponent(new CameraComponent(Matrix4().initPerspective(toRadians(70.0f), window.getAspectRatio(), 0.1f, 1000.0f)));
	//cameraObj->getTransform()->setPosition(Vector3(0, 20, 0));
	//cameraObj->getTransform()->rotate(Quaternion(AXIS_X, toRadians(90)));

	GameObject* dirLightObj = new GameObject(Vector3(0, 0, 0), Quaternion(), 2);
	dirLightObj->getTransform()->rotate(Quaternion());
	dirLightObj->addComponent(new DirectionalLight(COLOUR_WHITE, 0.04f, 10, 80.0f, 1.0f));

	SpriteSheet spriteSheet("someSprite", braid, 12, 1);
	spriteSheet.cycleDown();

	PhysicsObject* planeObj = new PhysicsObject();
	planeObj->getTransform()->setPosition(Vector3(0, 10, 20));
	planeObj->getTransform()->rotate(Quaternion(Vector3(1, 0, 0), toRadians(-90)));
	planeObj->addComponent(new SpriteRenderer(spriteSheet));
	planeObj->addComponent(new SpriteAnimator(spriteSheet, 50));

	//!!!
	//addToScene((new GameObject(Vector3(0, -1, 5), Quaternion(), 32.0f))
	//	->addComponent(new MeshRenderer(Mesh("terrain02.obj"), Material("bricks"))));
	//
	///*addToScene((new GameObject(Vector3(7,0,7)))
	//	->addComponent(new PointLight(Colour(0,255,0), 0.4f, Attenuation(0,0,1))));*/

	/*addToScene((new GameObject(Vector3(100, 100, 100), Quaternion(Vector3(1,0,0), toRadians(-45))))
		->addComponent(new DirectionalLight(COLOUR_WHITE, 0.4f, 10, 80.0f, 1.0f)));*/

	/*addToScene((new GameObject(Vector3(20,-11.0f,5), Quaternion(Vector3(1,0,0), toRadians(-60.0f)) * Quaternion(Vector3(0,1,0), toRadians(90.0f))))
		->addComponent(new SpotLight(Colour(0,255,255), 0.4f, Attenuation(0,0,0.02f), toRadians(91.1f), 7, 1.0f, 0.5f)));*/
		
	addToScene((new GameObject(Vector3(0, 2, 0), Quaternion(Vector3(0,1,0), 0.4f), 1.0f))
		->addComponent(new MeshRenderer(Mesh("plane3.obj"), Material("bricks2")))
		->addComponent(new FreeMove(10.0f, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT))
		->addChild((new GameObject(Vector3(0, 0, 25)))
			->addComponent(new MeshRenderer(Mesh("plane3.obj"), Material("bricks2")))));
	
	addToScene((new GameObject(Vector3(24,-12,5), Quaternion(Vector3(0,1,0), toRadians(30.0f))))
		->addComponent(new MeshRenderer(Mesh("cube.obj"), Material("bricks2"))));
		
	//addToScene((new GameObject(Vector3(0,0,7), Quaternion(), 1.0f))
	//	->addComponent(new MeshRenderer(Mesh("cube"), Material("bricks2"))));

	//TODO: temp
	//PhysicsEngine physicsEngine;
	//PhysicsObject p1 = PhysicsObject(new BoundingSphere(Vector3(-0.5f, 0, -10), 1.0f), Vector3(0, 0, 20));
	//PhysicsObject p2 = PhysicsObject(new BoundingSphere(Vector3(0.5f, 0, 10), 1.0f), Vector3(0, 0, -2));
	//physicsEngine.addObject(p1);
	//physicsEngine.addObject(p2);

	//PhysicsEngineComponent* physicsEngineComponent = new PhysicsEngineComponent(physicsEngine);

	//for(int i = 0; i < physicsEngineComponent->getPhysicsEngine().getNumObjects(); i++)
	//{
	//	addToScene((new GameObject(Vector3(0, 0, 0), Quaternion(), 1))
	//		->addComponent(new PhysicsObjectComponent(&physicsEngineComponent->getPhysicsEngine().getObject(i))));
	//		//->addComponent(new MeshRenderer(Mesh("sphere.obj"), Material("bricks"))));
	//}

	//addToScene((new GameObject())->addComponent(physicsEngineComponent));

	PhysicsObject* collTest1 = new PhysicsObject(Vector3(0.5f, 0, 20));
	collTest1->setVelocity(Vector3(0, 0, -2));
	collTest1->setCollider(new BoundingSphere(1));
	//collTest1->addComponent(new MeshRenderer(Mesh("cube.obj"), MATERIAL_DEFAULT));
	//collTest1->setCollider(new AABB(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));

	PhysicsObject* collTest2 = new PhysicsObject(Vector3(0, 0, 0));
	collTest2->setVelocity(Vector3(0, 0, 2));
	collTest2->setCollider(new BoundingSphere(1));
	//collTest2->setCollider(new AABB(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));

	//collTest2->addComponent(new FreeMove(10, Input::KEY_NUM8, Input::KEY_NUM5, Input::KEY_NUM4, Input::KEY_NUM6));

	//planeObj->addComponent(new Script("test.lua"));
	planeObj->addComponent(new FreeMove(10, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT));

	RigidBody* rigidBody = new RigidBody(4);
	rigidBody->m_linearDamping = 0.1f;
	rigidBody->m_angularDamping = 0.9f;

	planeObj->setRigidBody(rigidBody);
	planeObj->setCollider(new BoundingSphere(2));

	//addToScene2(collTest1);
	//addToScene2(collTest2);
	addToScene(cameraObj);
	addToScene(dirLightObj);
	addToScene2(planeObj);
	terrainObj->getTransform()->setScale(32);
	addToScene(terrainObj);

	PhysicsObject* planeObj2 = new PhysicsObject();
	planeObj2->getTransform()->setPosition(Vector3(0, -10, 20));
	planeObj2->getTransform()->rotate(Quaternion(Vector3(1, 0, 0), toRadians(-90)));
	planeObj2->addComponent(new SpriteRenderer(spriteSheet));
	planeObj2->addComponent(new SpriteAnimator(spriteSheet, 50));

	RigidBody* rigidBody2 = new RigidBody(-1);
	rigidBody2->m_linearDamping = 0.1f;
	rigidBody2->m_angularDamping = 0.9f;

	planeObj2->setRigidBody(rigidBody2);
	planeObj2->setCollider(new BoundingSphere(2));

	addToScene2(planeObj2);
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