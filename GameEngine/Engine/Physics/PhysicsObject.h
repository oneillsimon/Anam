#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "../Core/GameObject.h"
#include "CollisionFine.h"
#include "RigidBody.h"

class PhysicsObject : public GameObject
{
public:
	Collider* m_collider;

	PhysicsObject(RigidBody* rigidBody, Collider* collider, const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(0, 0, 0, 1), float scale = 1.0f);

	virtual void update(float delta);
};

#endif