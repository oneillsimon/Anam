#ifndef PHYSICSOBJECTCOMPONENT
#define PHYSICSOBJECTCOMPONENT

#include "../Physics/PhysicsEngine.h"
#include "GameComponent.h"

class PhysicsObjectComponent : public GameComponent
{
private:
	const PhysicsObject* m_physicsObject;

public:
	PhysicsObjectComponent(const PhysicsObject* object);

	virtual void update(float delta);
};

#endif