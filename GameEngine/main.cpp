#include "Engine\GameEngine.h"
#include "Engine\Core\GameMath.h"
#include "Engine\Components\FreeLook.h"
#include "Engine\Components\FreeMove.h"

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

void TestGame::init(const Window& window)
{
	Material bricks("bricks", Texture("bricks.jpg"), 0.5f, 4, Texture("bricks_normal.jpg"), Texture("bricks_disp.png"), 0.03f, -0.5f);
	Material bricks2("bricks2", Texture("bricks2.jpg"), 1, 8, Texture("bricks2_normal.jpg"), Texture("bricks2_disp.jpg"), 0.04f, -1.0f);

	IndexedModel square;
	{
	square.addVertex(1.0f, -1.0f, 0.0f); square.addTexCoord(1.0f, 1.0f);
	square.addVertex(1.0f, 1.0f, 0.0f); square.addTexCoord(1.0f, 0.0f);
	square.addVertex(-1.0f, -1.0f, 0.0f); square.addTexCoord(0.0f, 1.0f);
	square.addVertex(-1.0f, 1.0f, 0.0f); square.addTexCoord(0.0f, 0.0f);
	square.addFace(0, 1, 2); square.addFace(2, 1, 3);
	}

	Mesh customMesh("square", square.finalize());

	/*GameObject* objTerrain = new GameObject(Vector3(0, -1, 5), Quaternion(), 32.0f);
	objTerrain->addComponent(new MeshRenderer(Mesh("terrain02.obj"), Material("bricks.jpg")));

	addToScene(objTerrain);*/

	addToScene((new GameObject(Vector3(0, -1, 5), Quaternion(), 32.0f))
		->addComponent(new MeshRenderer(Mesh("terrain02.obj"), Material("bricks"))));

	addToScene((new GameObject(Vector3(0, -1, 5), Quaternion(), 32.0f))
		->addComponent(new FreeLook(window.getCentre()))
		->addComponent(new FreeMove(10.0f, Input::KEY_W, Input::KEY_S, Input::KEY_A, Input::KEY_D))
		->addComponent(new CameraComponent(Matrix4().initPerspective(GameMath::toRadians(70.0f), window.getAspectRatio(), 0.1f, 1000.0f))));
	
	addToScene((new GameObject(Vector3(7,0,7)))
		->addComponent(new PointLight(Colour(0,255,0), 0.4f, Attenuation(0,0,1))));

	addToScene((new GameObject(Vector3(), Quaternion(Vector3(1,0,0), GameMath::toRadians(-45))))
		->addComponent(new DirectionalLight(Colour(255,255,255), 0.4f, 10, 80.0f, 1.0f)));

	addToScene((new GameObject(Vector3(20,-11.0f,5), Quaternion(Vector3(1,0,0), GameMath::toRadians(-60.0f)) * Quaternion(Vector3(0,1,0), GameMath::toRadians(90.0f))))
		->addComponent(new SpotLight(Colour(0,255,255), 0.4f, Attenuation(0,0,0.02f), GameMath::toRadians(91.1f), 7, 1.0f, 0.5f)));
		
	addToScene((new GameObject(Vector3(0, 2, 0), Quaternion(Vector3(0,1,0), 0.4f), 1.0f))
		->addComponent(new MeshRenderer(Mesh("plane3.obj"), Material("bricks2")))
		->addChild((new GameObject(Vector3(0, 0, 25)))
			->addComponent(new MeshRenderer(Mesh("plane3.obj"), Material("bricks2")))));
	
	addToScene((new GameObject(Vector3(24,-12,5), Quaternion(Vector3(0,1,0), GameMath::toRadians(30.0f))))
		->addComponent(new MeshRenderer(Mesh("cube.obj"), Material("bricks2"))));
		
	addToScene((new GameObject(Vector3(0,0,7), Quaternion(), 1.0f))
		->addComponent(new MeshRenderer(Mesh("square"), Material("bricks2"))));
}

int main()
{
	TestGame game;
	CoreEngine engine(800, 600, 60, &game);
	engine.createWindow("Game ENGINE");
	engine.start();

	return 0;
}

//int main()
//{
//	/*TestGame game;
//	CoreEngine engine(800, 600, 120, &game);
//	engine.createWindow("Game Engine");
//	engine.start();*/
//
//	TestGame game;
//	
//	return 0;
//}