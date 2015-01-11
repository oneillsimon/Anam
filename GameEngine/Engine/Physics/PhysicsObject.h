#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "Octree/Octree.h"
#include "../Core/GameObject.h"

class PhysicsObject : public GameObject
{
public:
	Vector3 velocity;
	float r;

	PhysicsObject(const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(), const Vector3& scale = Vector3());
};

#endif