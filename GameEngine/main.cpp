#include "Engine\GameEngine.h"
#include "Engine\Core\GameMath.h"
#include "Engine\Components\FreeLook.h"
#include "Engine\Components\FreeMove.h"
#include "Engine\Physics\Particle.h"

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
	Particle* testParticle = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 10), 1.0f, 0.9f);

	//planeObj->addComponent(new MeshRenderer(new Mesh("./res/models/plane3.obj"), new Material(new Texture("bricks.jpg"), 1, 8)));
	planeObj->addComponent(new MeshRenderer(new Mesh("./res/models/plane3.obj"), new Material(new Texture("bricks.jpg"), 1, 8), new Vector4(38.0f / 255.0f, 211.0f / 255.0f, 188.0f / 255.0f, 127.5f / 255.0f)));
	planeObj->getTransform().setPosition(Vector3(0, -1, 5));
	planeObj->getTransform().setScale(Vector3(4, 4, 4));

	cameraObj->addComponent(new Camera(Matrix4().initPerspective((float)GameMath::toRadians(70.0), Window::getAspectRatio(), 0.1f, 1000.0f)));
	cameraObj->addComponent(new MeshRenderer(new Line(Vector3(0, 2, 15), Vector3(1, 0, 15)), new Vector4(1, 0, 0, 1)));
	cameraObj->getTransform().setPosition(Vector3(0, 15, 0));
	cameraObj->addComponent(new FreeLook());
	cameraObj->addComponent(new FreeMove(20));

	testParticle->addComponent(new MeshRenderer(new Mesh("./res/models/plane3.obj"), new Material(new Texture("bricks.jpg"), 1, 8)));
	testParticle->getTransform().setPosition(Vector3(0, 0, 5));
	testParticle->getTransform().setScale(Vector3(1, 1, 1));
	testParticle->addComponent(new FreeMove(10.0f, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT));

	addToScene(planeObj);
	addToScene(cameraObj);
	//addToScene(testParticle);

	// Lighting

	GameObject* pointlightObj = new GameObject();
	pointlightObj->addComponent(new PointLight(Vector3(0, 1, 0), 1.4f, Attenuation(0, 0, 1)));
	pointlightObj->getTransform().setPosition(Vector3(7, 0, 7));
	addToScene(pointlightObj);

	GameObject* spotlightObj = new GameObject();
	spotlightObj->addComponent(new SpotLight(Vector3(0, 1, 1), 1.0f, Attenuation(0, 0, 0.1f), 0.7f));
	spotlightObj->addComponent(new FreeMove(10, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT));
	spotlightObj->getTransform().setRotation(Quaternion(Vector3(0, 1, 0), GameMath::toRadians(90.0f)));
	addToScene(spotlightObj);

	// TODO: intensity on directional light doesn't work
	GameObject* dirlightObj = new GameObject();
	dirlightObj->addComponent(new DirectionalLight(Vector3(1, 1, 1), 0.0f));
	dirlightObj->getTransform().setRotation(Quaternion(Vector3(1, 0, 0), GameMath::toRadians(-45)));
	//addToScene(dirlightObj);
}

int main()
{
	TestGame game;
	CoreEngine engine(800, 600, 60, &game);
	engine.createWindow("Game Engine");
	engine.start();
	
	return 0;
}
