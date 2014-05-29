#include "Engine\GameEngine.h"
#include "Engine\Core\GameMath.h"
#include "Engine\Components\FreeLook.h"
#include "Engine\Components\FreeMove.h"

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
	GameObject* testmesh1 = new GameObject();
	GameObject* testmesh2 = new GameObject();

	planeObj->addComponent(new MeshRenderer(new Mesh("./res/models/plane3.obj"), new Material(new Texture("bricks.jpg"), 1, 8)));
	planeObj->getTransform().setPosition(Vector3(0, -1, 5));
	planeObj->getTransform().setScale(Vector3(4, 4, 4));

	cameraObj->addComponent(new Camera(Matrix4().initPerspective((float)GameMath::toRadians(70.0), Window::getAspectRatio(), 0.1f, 1000.0f)));
	cameraObj->getTransform().setPosition(Vector3(0, 15, 0));
	cameraObj->addComponent(new FreeLook());
	cameraObj->addComponent(new FreeMove());

	testmesh1->addComponent(new MeshRenderer(new Mesh("./res/models/plane3.obj"), new Material(new Texture("bricks.jpg"), 1, 8)));
	testmesh2->addComponent(new MeshRenderer(new Mesh("./res/models/plane3.obj"), new Material(new Texture("bricks.jpg"), 1, 8)));

	testmesh1->getTransform().setPosition(Vector3(0, 2, 0));
	testmesh1->getTransform().setRotation(Quaternion(Vector3(0, 1, 0), 0.4f));
	testmesh1->getTransform().setScale(Vector3(1, 1, 1));

	testmesh2->getTransform().setPosition(Vector3(0, 0, 25));

	testmesh1->addChild(testmesh2);
	testmesh1->addComponent(new FreeMove(10.0f, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT));

	addToScene(planeObj);
	addToScene(cameraObj);
	addToScene(testmesh1);

	// Lighting

	GameObject* pointlightObj = new GameObject();
	pointlightObj->addComponent(new PointLight(Vector3(0, 1, 0), 0.4f, Attenuation(0, 0, 1)));
	pointlightObj->getTransform().setPosition(Vector3(7, 0, 7));
	addToScene(pointlightObj);

	GameObject* spotlightObj = new GameObject();
	spotlightObj->addComponent(new SpotLight(Vector3(0, 1, 1), 1.0f, Attenuation(0, 0, 0.1f), 0.7f));
	spotlightObj->getTransform().setRotation(Quaternion(Vector3(0, 1, 0), GameMath::toRadians(90.0f)));
	addToScene(spotlightObj);

	// TODO: intensity on directional light doesn't work
	GameObject* dirlightObj = new GameObject();
	dirlightObj->addComponent(new DirectionalLight(Vector3(1, 0, 0.05f), 0.0f));
	dirlightObj->getTransform().setRotation(Quaternion(Vector3(1, 0, 0), GameMath::toRadians(-45)));
	addToScene(dirlightObj);
}

int main()
{
	TestGame game;
	CoreEngine engine(800, 600, 60, &game);
	engine.createWindow("Game Engine");
	engine.start();
	
	return 0;
}
