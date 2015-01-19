#ifndef COLLIDEFINE_H
#define COLLIDEFINE_H

#include "Contacts.h"
#include "../Core/GameComponent.h"

class IntersectionTests;
class CollisionDetector;

class CollisionPrimitive : public GameComponent
{
public:
	friend class IntersectionTests;
	friend class CollisionDetector;

	RigidBody* m_body;
	Matrix4 m_offset;

	void calculateInternals();
	Vector3 getAxis(unsigned index) const
	{
		return m_parent->getTransform()->getTransformation().getAxisVector(index);
	}
};

class CollisionSphere : public CollisionPrimitive
{
public:
	float m_radius;
};

class CollisionPlane
{
public:
	Vector3 m_direction;
	float m_offset;
};

class CollisionBox : public CollisionPrimitive
{
public:
	Vector3 m_halfSize;
};

class IntersectionTests
{
public:
	static bool sphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane);
	static bool sphereAndSphere(const CollisionSphere& one, const CollisionSphere& two);
	static bool boxAndBox(const CollisionBox& one, const CollisionBox& two);
	static bool boxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane);
};

class CollisionData
{
public:
	Contact* m_contactArray;
	Contact* m_contacts;
	int m_contactsLeft;
	unsigned m_contactCount;
	float m_friction;
	float m_restitution;
	float m_tolerance;

	bool hasMoreContacts()
	{
		return m_contactsLeft > 0;
	}

	void reset(unsigned maxContacts)
	{
		m_contactsLeft = maxContacts;
		m_contactCount = 0;
		m_contacts = m_contactArray;
	}

	void addContacts(unsigned count)
	{
		m_contactsLeft -= count;
		m_contactCount += count;
		m_contacts += count;
	}
};

class CollisionDetector
{
public:
	static unsigned sphereAndHalfSpace(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data);
	static unsigned sphereAndTruePlane(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data);
	static unsigned sphereAndSphere(const CollisionSphere& one, const CollisionSphere& two, CollisionData* data);
	static unsigned boxAndHalfSpace(const CollisionBox& box, const CollisionPlane& plane, CollisionData* data);
	static unsigned boxAndBox(const CollisionBox& one, const CollisionBox& two, CollisionData* data);
	static unsigned boxAndPoint(const CollisionBox& box, const Vector3& point, CollisionData* data);
	static unsigned boxAndSphere(const CollisionBox& box, const CollisionSphere& sphere, CollisionData* data);
};

#endif