#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "../../Core/GameComponent.h"
#include "../../Physics/PhysicsObject.h"

class RigidBodyComponent : public GameComponent
{
public:
	PhysicsObject* m_obj;
	RigidBodyComponent(PhysicsObject* obj);

	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
};

#endif