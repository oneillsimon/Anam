#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "../Physics_/RigidBody.h"
#include "../Core/GameObject.h"
#include "../Physics_/CollideFine.h"

class PhysicsObject : public GameObject
{
public:
	//RigidBody* m_rigidBody;
	//Collider* m_collider;
	CollisionSphere* collider;

	PhysicsObject(RigidBody* rigidBody, const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(0, 0, 0, 1), float scale = 1.0f);

	virtual void update(float delta);
};

#endif