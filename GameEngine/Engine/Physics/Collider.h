#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Core/Math3D.h"
#include "../Core/Transform.h"

class PhysicsObject;

class Collider
{
public:
	enum Type
	{
		SPHERE,
		BOX,
		PLANE,
		OBB
	};

	int m_type;
	PhysicsObject* m_parent;
	bool m_isColliding;

	virtual void collide(PhysicsObject& p1, CollisionData_& data);

	Vector3 getAxis(unsigned index) const;
};

class ColliderSphere : public Collider
{
public:
	ColliderSphere(float radius = 1.0f);
	float m_radius;

	virtual void collide(PhysicsObject& p1, CollisionData_& data);
};

class ColliderBox : public Collider
{
public:
	ColliderBox(const Vector3& halfSize = Vector3(1, 1, 1));
	Vector3 m_halfSize;

	virtual void collide(PhysicsObject& p1, CollisionData_& data);
};

class ColliderPlane : public Collider
{
public:
	ColliderPlane(const Vector3& normal = Vector3(0, 1, 0), float distance = 0.0f);
	Vector3 m_normal;
	float m_distance;

	virtual void collide(PhysicsObject& p1, CollisionData_& data);
};

class ColliderOBB : public Collider
{
public:
	ColliderOBB(const Vector3& halfExtents = Vector3(1, 1, 1));
	Vector3 e;

	virtual void collide(PhysicsObject& p1, CollisionData_& data);
};

#endif