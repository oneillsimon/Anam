#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "Octree/Octree.h"
#include "../Physics_/RigidBody.h"
#include "../Core/GameObject.h"

class PhysicsObject : public GameObject
{
public:
	RigidBody* m_rigidBody;
	Collider* m_collider;

	PhysicsObject(RigidBody* rigidBody, Collider* collider, const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(), float scale = 1.0f);

	virtual void update(float delta);
};

#endif