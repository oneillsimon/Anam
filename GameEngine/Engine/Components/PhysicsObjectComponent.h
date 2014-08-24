#ifndef PHYSICSOBJECTCOMPONENT
#define PHYSICSOBJECTCOMPONENT

#include "../Components/GameComponent.h"
#include "../Physics/PhysicsEngine.h"

class PhysicsObjectComponent : public GameComponent
{
private:
	const PhysicsObject* m_physicsObject;

public:
	PhysicsObjectComponent(const PhysicsObject* object);

	virtual void update(float delta);
};

#endif