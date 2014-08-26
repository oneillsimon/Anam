#include "Inneall.h"

#include <iostream>

#include "Engine\Components\PhysicsEngineComponent.h"
#include "Engine\Components\PhysicsObjectComponent.h"

#include "Engine\Rendering\SpriteSheet.h"
#include "Engine\Components\SpriteRenderer.h"
#include "Engine\Components\SpriteAnimator.h"

#undef main

class TestGame : public Game
{
public:
	TestGame() { }
	
	virtual void init(const Window& window);
protected:
private:
	TestGame(const TestGame& other) {}
	void operator =(const TestGame& other) {}
};

static void tests();

void TestGame::init(const Window& window)
{
	Material bricks("bricks", Texture("bricks.jpg"), 0.5f, 4, Texture("bricks_normal.jpg"), Texture("bricks_disp.png"), 0.03f, -0.5f, COLOUR_LIGHT_STEEL_BLUE);
	Material bricks_("bricks_", Texture("bricks.jpg"), 0.5f, 4, Texture("bricks_normal.jpg"), Texture("bricks_disp.png"), 0.03f, -0.5f, COLOUR_FOREST_GREEN);
	Material bricks2("bricks2", Texture("bricks2.jpg"), 1, 8, Texture("bricks2_normal.jpg"), Texture("bricks2_disp.jpg"), 0.04f, -1.0f);
	Material blank("blank", Texture("white.png"));
	Material default("default", Texture(""));
	MeshRenderer* terrainRenderer = new MeshRenderer(Mesh("terrain02.obj"), Material("bricks"));

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

	GameObject* dirLightObj = new GameObject(Vector3(0, 0, 0), Quaternion(), 2);
	dirLightObj->getTransform()->rotate(Quaternion());
	dirLightObj->addComponent(new DirectionalLight(COLOUR_WHITE, 0.4f, 10, 80.0f, 1.0f));

	SpriteSheet spriteSheet("someSprite", Material("default"), 8, 8);

	GameObject* planeObj = new GameObject();
	planeObj->getTransform()->setPosition(Vector3(0, 0, 2));
	planeObj->getTransform()->rotate(Quaternion(Vector3(1, 0, 0), toRadians(-180)));
	planeObj->addComponent(new SpriteRenderer(spriteSheet));
	planeObj->addComponent(new SpriteAnimator(spriteSheet, 500, true));

	addToScene((new GameObject(Vector3(0, -1, 5), Quaternion(), 32.0f))
		->addComponent(new MeshRenderer(Mesh("terrain02.obj"), Material("bricks"))));
	
	/*addToScene((new GameObject(Vector3(7,0,7)))
		->addComponent(new PointLight(Colour(0,255,0), 0.4f, Attenuation(0,0,1))));*/

	addToScene((new GameObject(Vector3(100, 100, 100), Quaternion(Vector3(1,0,0), toRadians(-45))))
		->addComponent(new DirectionalLight(COLOUR_CORNFLOWER_BLUE, 0.4f, 10, 80.0f, 1.0f)));

	/*addToScene((new GameObject(Vector3(20,-11.0f,5), Quaternion(Vector3(1,0,0), toRadians(-60.0f)) * Quaternion(Vector3(0,1,0), toRadians(90.0f))))
		->addComponent(new SpotLight(Colour(0,255,255), 0.4f, Attenuation(0,0,0.02f), toRadians(91.1f), 7, 1.0f, 0.5f)));*/
		
	/*addToScene((new GameObject(Vector3(0, 2, 0), Quaternion(Vector3(0,1,0), 0.4f), 1.0f))
		->addComponent(new MeshRenderer(Mesh("plane3.obj"), Material("bricks2")))
		->addComponent(new FreeMove(10.0f, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT))
		->addChild((new GameObject(Vector3(0, 0, 25)))
			->addComponent(new MeshRenderer(Mesh("plane3.obj"), Material("bricks2")))));*/
	
	/*addToScene((new GameObject(Vector3(24,-12,5), Quaternion(Vector3(0,1,0), toRadians(30.0f))))
		->addComponent(new MeshRenderer(Mesh("cube.obj"), Material("bricks2"))));
		
	addToScene((new GameObject(Vector3(0,0,7), Quaternion(), 1.0f))
		->addComponent(new MeshRenderer(Mesh("cube"), Material("bricks2"))));*/

	//TODO: temp
	PhysicsEngine physicsEngine;
	//physicsEngine.addObject(PhysicsObject(new BoundingSphere(Vector3(0.5f, 0, 0), 2.0f), Vector3(0, 0, 1)));
	//physicsEngine.addObject(PhysicsObject(new BoundingSphere(Vector3(0, 0, 10), 1.0f), Vector3(0, 0, -1)));

	PhysicsEngineComponent* physicsEngineComponent = new PhysicsEngineComponent(physicsEngine);

	for(int i = 0; i < physicsEngineComponent->getPhysicsEngine().getNumObjects(); i++)
	{
		if(i == 0)
		addToScene((new GameObject(Vector3(0, 0, 0), Quaternion(), 2.0f))
			->addComponent(new PhysicsObjectComponent(&physicsEngineComponent->getPhysicsEngine().getObject(i)))
			->addComponent(new MeshRenderer(Mesh("sphere.obj"), Material("bricks"))));
		else
			addToScene((new GameObject(Vector3(0, 0, 0), Quaternion(), 1.0f))
			->addComponent(new PhysicsObjectComponent(&physicsEngineComponent->getPhysicsEngine().getObject(i)))
			->addComponent(new MeshRenderer(Mesh("sphere.obj"), Material("bricks_"))));
	}

	//addToScene((new GameObject())->addComponent(physicsEngineComponent));

	addToScene(cameraObj);
	addToScene(dirLightObj);
	addToScene(planeObj);
}

int main()
{
	//tests();
	//getchar();

	TestGame game;
	CoreEngine engine(800, 600, 60, &game);
	engine.createWindow("Game ENGINE");
	engine.start();

	return 0;
}

void tests()
{
	BoundingSphere sphere1(Vector3(0, 0, 0), 1.0f);
	BoundingSphere sphere2(Vector3(0, 3, 0), 1.0f);
	BoundingSphere sphere3(Vector3(0, 0, 2), 1.0f);
	BoundingSphere sphere4(Vector3(1, 0, 0), 1.0f);

	IntersectionData sphere1IntSphere2 = sphere1.intersectBoundingSphere(sphere2);
	IntersectionData sphere1IntSphere3 = sphere1.intersectBoundingSphere(sphere3);
	IntersectionData sphere1IntSphere4 = sphere1.intersectBoundingSphere(sphere4);

	std::cout << "Sphere1 intersect Sphere2: " << sphere1IntSphere2.getDoesIntersect() << ", Distance: " << sphere1IntSphere2.getDistance() <<std::endl;
	std::cout << "Sphere1 intersect Sphere3: " << sphere1IntSphere3.getDoesIntersect() << ", Distance: " << sphere1IntSphere3.getDistance() <<std::endl;
	std::cout << "Sphere1 intersect Sphere4: " << sphere1IntSphere4.getDoesIntersect() << ", Distance: " << sphere1IntSphere4.getDistance() <<std::endl;

	AABB aabb1(Vector3(0, 0, 0), Vector3(1, 1, 1));
	AABB aabb2(Vector3(1, 1, 1), Vector3(2, 2, 2));
	AABB aabb3(Vector3(1, 0, 0), Vector3(2, 1, 1));
	AABB aabb4(Vector3(0, 0, -2), Vector3(1, 1, -1));
	AABB aabb5(Vector3(0, 0.5f, 0), Vector3(1, 1.5f, 1));

	IntersectionData aabb1Intaabb2 = aabb1.intersectAABB(aabb2);
	IntersectionData aabb1Intaabb3 = aabb1.intersectAABB(aabb3);
	IntersectionData aabb1Intaabb4 = aabb1.intersectAABB(aabb4);
	IntersectionData aabb1Intaabb5 = aabb1.intersectAABB(aabb5);

	std::cout << "AABB1 intersect AABB2: " << aabb1Intaabb2.getDoesIntersect() << ", Distance: " << aabb1Intaabb2.getDistance() << std::endl;
	std::cout << "AABB1 intersect AABB3: " << aabb1Intaabb3.getDoesIntersect() << ", Distance: " << aabb1Intaabb3.getDistance() << std::endl;
	std::cout << "AABB1 intersect AABB4: " << aabb1Intaabb4.getDoesIntersect() << ", Distance: " << aabb1Intaabb4.getDistance() << std::endl;
	std::cout << "AABB1 intersect AABB5: " << aabb1Intaabb5.getDoesIntersect() << ", Distance: " << aabb1Intaabb5.getDistance() << std::endl;

	Plane plane1(Vector3(0, 1, 0), 0);

	IntersectionData plane1IntSphere1 = plane1.intersectSphere(sphere1);
	IntersectionData plane1IntSphere2 = plane1.intersectSphere(sphere2);
	IntersectionData plane1IntSphere3 = plane1.intersectSphere(sphere3);
	IntersectionData plane1IntSphere4 = plane1.intersectSphere(sphere4);

	std::cout << "Plane 1 intersect Sphere1: " << plane1IntSphere1.getDoesIntersect() << ", Distance: " << plane1IntSphere1.getDistance() << std::endl;
	std::cout << "Plane 1 intersect Sphere2: " << plane1IntSphere2.getDoesIntersect() << ", Distance: " << plane1IntSphere2.getDistance() << std::endl;
	std::cout << "Plane 1 intersect Sphere3: " << plane1IntSphere3.getDoesIntersect() << ", Distance: " << plane1IntSphere3.getDistance() << std::endl;
	std::cout << "Plane 1 intersect Sphere4: " << plane1IntSphere4.getDoesIntersect() << ", Distance: " << plane1IntSphere4.getDistance() << std::endl;
}