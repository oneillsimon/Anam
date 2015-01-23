#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Core/GameComponent.h"
#include "../Core/Math3D.h"
#include "../Core/Transform.h"
#include "RigidBody.h"

class IntersectionTests;
class CollisionDetector;

class Collider : public GameComponent
{
public:
	friend class IntersectionTests;
	friend class CollisionDetector;

	enum Type
	{
		SPHERE,
		BOX,
		PLANE
	};

	RigidBody* m_body;
	Matrix4 m_offset;
	int m_type;

	void calculateInternals();
	Vector3 getAxis(unsigned index) const
	{
		return m_parent->getTransform()->getTransformation().getAxisVector(index);
	}

	virtual Vector3 getExtents() = 0;
};

class ColliderSphere : public Collider
{
public:
	ColliderSphere(float radius = 1.0f);
	float m_radius;

	virtual Vector3 getExtents();
};

class ColliderPlane : public Collider
{
public:
	ColliderPlane(const Vector3& direction, float offset);
	Vector3 m_direction;
	float m_offset;

	virtual Vector3 getExtents()
	{
		return Vector3();
	}
};

class ColliderBox : public Collider
{
public:
	ColliderBox(const Vector3& halfSize = Vector3(1, 1, 1));
	Vector3 m_halfSize;

	virtual Vector3 getExtents();
};

#endif