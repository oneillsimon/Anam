#include "../Core/GameComponent.h"
#include "../Physics/PhysicsObject.h"

class TestComponent : public GameComponent
{
public:
	int number;
	PhysicsObject obj;

	TestComponent()
	{

	}

	virtual void update(float delta)
	{
		obj = *(PhysicsObject*)m_parent;
		obj.getRigidBody()->addAngularForce(AXIS_Y * 5);
	}

	virtual void input(const Input& input, float delta)
	{
		if(input.getKey(Input::KEY_Y))
		{
			obj.setVelocity(Vector3(0, 0, 2));
		}
	}
};