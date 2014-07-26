#include "Engine\GameEngine.h"
#include "Engine\Core\GameMath.h"
#include "Engine\Components\FreeLook.h"
#include "Engine\Components\FreeMove.h"
#include "Engine\Physics\Particle.h"
#include "Engine\Rendering\LineMesh.h"
#include "Engine\Rendering\Colour.h"

class TestGame : public Game
{
public:
	TestGame() { }
	
	virtual void init();
protected:
private:
	TestGame(const TestGame& other) {}
	void operator =(const TestGame& other) {}
};

void TestGame::init()
{
	GameObject* planeObj = new GameObject();
	GameObject* cameraObj = new GameObject();
	GameObject* testMesh1 = new GameObject();
	GameObject* testMesh2 = new GameObject();

	planeObj->addComponent(new MeshRenderer(new Mesh("./res/models/plane4.obj"), new Material(new Texture("bricks.jpg"), 0.5f, 4,
																							  new Texture("bricks_normal.jpg"),
																							  new Texture("bricks_disp.png"), 0.03f, -0.5f)));
	planeObj->getTransform().setPosition(Vector3(0, -1, 5));
	planeObj->getTransform().setScale(Vector3(4, 4, 4));

	cameraObj->addComponent(new Camera(Matrix4().initPerspective((float)GameMath::toRadians(70.0), Window::getAspectRatio(), 0.1f, 1000.0f)));
	cameraObj->getTransform().setPosition(Vector3(0, 15, 0));
	cameraObj->addComponent(new FreeLook());
	cameraObj->addComponent(new FreeMove(20));

	testMesh1->addComponent(new MeshRenderer(new Mesh("./res/models/plane3.obj"), new Material(new Texture("bricks2.jpg"), 1, 8,
																							   new Texture("bricks2_normal.png"),
																							   new Texture("bricks2_disp.jpg"), 0.04f, -1.0f)));

	testMesh2->addComponent(new MeshRenderer(new Mesh("./res/models/plane3.obj"), new Material(new Texture("bricks2.jpg"), 1, 8,
																							   new Texture("bricks2_normal.png"),
																							   new Texture("bricks2_disp.jpg"), 0.04f, -1.0f)));

	testMesh1->getTransform().setPosition(Vector3(0, 2, 0));
	testMesh1->getTransform().setRotation(Quaternion(Vector3(0, 1, 0), 0.4f));
	testMesh1->addComponent(new FreeMove(10, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT));
	testMesh1->getTransform().setScale(1.0f);

	testMesh2->getTransform().setPosition(Vector3(0, 0, 25));
	testMesh2->addComponent(new FreeMove(15, Input::KEY_I, Input::KEY_K, Input::KEY_J, Input::KEY_L));
	testMesh1->addChild(testMesh2);

	addToScene(planeObj);
	addToScene(cameraObj);
	addToScene(testMesh1);

	// Lighting

	GameObject* pointlightObj = new GameObject();
	pointlightObj->addComponent(new PointLight(Colour(0, 255, 0), 0.4f, Attenuation(0, 0, 1)));
	pointlightObj->getTransform().setPosition(Vector3(7, 0, 7));
	//addToScene(pointlightObj);
	
	GameObject* spotlightObj = new GameObject();
	spotlightObj->addComponent(new SpotLight(Colour(0, 255, 255), 0.4f, Attenuation(0, 0, 0.1f), 0.7f));
	spotlightObj->getTransform().setRotation(Quaternion(Vector3(0, 1, 0), GameMath::toRadians(90)));
	spotlightObj->addComponent(new FreeMove(10, Input::KEY_T, Input::KEY_G, Input::KEY_F, Input::KEY_H));
	//addToScene(spotlightObj);
	
	GameObject* dirlightObj = new GameObject();
	dirlightObj->addComponent(new DirectionalLight(Colour(255, 255, 255), 0.4f));
	dirlightObj->getTransform().setRotation(Quaternion(Vector3(1, 0, 0), GameMath::toRadians(-45)));
	addToScene(dirlightObj);
}

int main()
{
	TestGame game;
	CoreEngine engine(800, 600, 120, &game);
	engine.createWindow("Game Engine");
	engine.start();
	
	return 0;
}