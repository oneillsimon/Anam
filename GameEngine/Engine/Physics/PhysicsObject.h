#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "Octree/Octree.h"
#include "../Physics_/RigidBody.h"
#include "../Core/GameObject.h"

class PhysicsObject : public GameObject
{
public:
	RigidBody* m_rigidBody;
	float r;

	PhysicsObject(RigidBody* rigidBody, const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(), float scale = 1.0f);
};

#endif