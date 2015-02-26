#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "../Core/GameComponent.h"
#include "Collider.h"

class Collider;
class RigidBody;

class PhysicsComponent : public GameComponent
{
private:
	Collider* m_collider;

public:
	PhysicsComponent(Collider* collider, RigidBody* rigidBody = 0);

	virtual void initialise();
	virtual void update(float delta);
	
	Collider* getCollider();
	RigidBody* getBody();
};

#endif