#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "../../Core/GameComponent.h"
#include "../../Physics/RigidBody.h"

class RigidBodyComponent : public GameComponent
{
public:
	RigidBody* m_body;
	RigidBodyComponent(RigidBody* body);

	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
};

#endif