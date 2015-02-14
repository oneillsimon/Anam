#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "../Core/GameObject.h"
#include "Collider.h"

#define LINEAR_VELOCITY_MIN 0.00001

class Collider;

class PhysicsObject : public GameObject
{
private:
	Collider* m_collider;
	
public:

	PhysicsObject(const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(0, 0, 0, 1), float scale = 1.0f);

	virtual void initialise();
	virtual void update(float delta);

	Collider* getCollider();
};

#endif