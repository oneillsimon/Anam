#include "../Core/GameComponent.h"
#include "../Core/CoreEngine.h"
#include "Collider.h"

class PhysicsComponent : public GameComponent
{
private:
	Collider* m_collider;

public:
	PhysicsComponent(Collider* collider, RigidBody* rigidBody = 0);

	virtual void initialise();
	virtual void update(float delta);
};