#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "../Physics_/RigidBody.h"
#include "../Core/GameObject.h"
#include "../Physics_/CollideFine.h"

class PhysicsObject : public GameObject
{
public:
	CollisionPrimitive* m_collider;

	PhysicsObject(RigidBody* rigidBody, CollisionPrimitive* collider, const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(0, 0, 0, 1), float scale = 1.0f);

	virtual void update(float delta);
};

#endif