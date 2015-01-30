#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Core/Math3D.h"
#include "../Core/Transform.h"
#include "RigidBody.h"

class IntersectionTests;
class CollisionDetector;
class CollisionData;
class PhysicsObject;

class Collider
{
public:
	friend class IntersectionTests;
	friend class CollisionDetector;

	enum Type
	{
		SPHERE,
		BOX,
	};

	Transform* m_owner;
	RigidBody* m_body;
	Matrix4 m_offset;
	int m_type;

	void calculateInternals();
	Vector3 getAxis(unsigned index) const
	{
		return m_owner->getTransformation().getAxisVector(index);
	}

	//virtual void collide(Collider& collider, CollisionData* data) = 0;
	static void collide(PhysicsObject& p0, PhysicsObject& p1, CollisionData* data);
};

class ColliderSphere : public Collider
{
public:
	ColliderSphere(float radius = 1.0f);
	float m_radius;

	//st void collide(Collider& collider, CollisionData* data);
};

class ColliderBox : public Collider
{
public:
	ColliderBox(const Vector3& halfSize = Vector3(1, 1, 1));
	Vector3 m_halfSize;
};

class ColliderPlane
{
public:
	ColliderPlane(const Vector3& direction, float offset);
	Vector3 m_direction;
	float m_offset;
};

#endif