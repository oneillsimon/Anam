#include "Engine\GameEngine.h"
#include "Engine\Core\GameMath.h"
#include "Engine\Components\FreeLook.h"
#include "Engine\Components\FreeMove.h"
#include "Engine\Physics\Particle.h"
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
	Particle* testParticle = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 10), 1.0f, 0.9f);

	//planeObj->addComponent(new MeshRenderer(new Mesh("./res/models/plane3.obj"), new Material(new Texture("bricks.jpg"), 1, 8)));
	planeObj->addComponent(new MeshRenderer(new Mesh("./res/models/plane3.obj"), new Colour(94, 213, 100, 0)));
	planeObj->getTransform().setPosition(Vector3(0, -1, 5));
	planeObj->getTransform().setScale(Vector3(4, 4, 4));

	cameraObj->addComponent(new Camera(Matrix4().initPerspective((float)GameMath::toRadians(70.0), Window::getAspectRatio(), 0.1f, 1000.0f)));
	//cameraObj->addComponent(new MeshRenderer(new Line(Vector3(0, 2, 15), Vector3(1, 0, 15)), new Colour(255, 0, 0)));
	//cameraObj->addComponent(new Line(Vector3(0, 2, 15), Vector3(1, 0, 15), new Colour(255, 0, 0)));
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
	pointlightObj->addComponent(new PointLight(Colour(255, 255, 255), 1.2f, Attenuation(0, 0, 1)));
	pointlightObj->getTransform().setPosition(Vector3(7, 0, 7));
	addToScene(pointlightObj);

	GameObject* spotlightObj = new GameObject();
	spotlightObj->addComponent(new SpotLight(Colour(255, 255, 255), 15.0f, Attenuation(0, 0, 1.0f), 0.7f));
	spotlightObj->addComponent(new FreeMove(10, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT));
	spotlightObj->getTransform().setRotation(Quaternion(Vector3(0, 1, 0), GameMath::toRadians(90.0f)));
	spotlightObj->getTransform().setScale(Vector3(2, 2, 2));
	addToScene(spotlightObj);

	GameObject* dirlightObj = new GameObject();
	dirlightObj->addComponent(new DirectionalLight(Colour(255, 0, 255, 255), 0));
	dirlightObj->addComponent(new FreeMove(10, Input::KEY_UP, Input::KEY_DOWN, Input::KEY_LEFT, Input::KEY_RIGHT));
	dirlightObj->getTransform().setRotation(Quaternion(Vector3(1, 0, 0), GameMath::toRadians(-45)));
	addToScene(dirlightObj);
	GameObject* test = new GameObject();
	//test->addComponent(new MeshRenderer(new Line(spotlightObj->getTransform().getPosition(), cameraObj->getTransform().getPosition()), new Colour(0, 255, 0)));
	//test->addComponent(new MeshRenderer(new Line(Vector3(0, 5, 15), Vector3(1, 7, 15)), new Colour(0, 255, 0)));
	//test->addComponent(new Line(spotlightObj->getTransform().getPosition(), cameraObj->getTransform().getPosition(), new Colour(0, 255, 0)));
	addToScene(test);
}

int main()
{
	TestGame game;
	CoreEngine engine(800, 600, 60, &game);
	engine.createWindow("Game Engine");
	engine.start();
	
	return 0;
}
