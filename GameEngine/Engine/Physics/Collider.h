#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Core/Math3D.h"
#include "../Core/Transform.h"

class PhysicsObject;

struct CollisionData_
{
	float m_penetration;
	Vector3 m_normal;
	Vector3 m_point;
};

class Collider
{
public:
	enum Type
	{
		SPHERE,
		BOX,
		PLANE
	};

	int m_type;

	static void collide(PhysicsObject& p0, PhysicsObject& p1);
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

class ColliderPlane : public Collider
{
public:
	ColliderPlane(const Vector3& normal, float distance);
	Vector3 m_normal;
	float m_distance;
};

#endif