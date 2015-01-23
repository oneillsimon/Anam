#ifndef COLLISIONFINE_H
#define COLLISIONFINE_H

#include "Collider.h"
#include "Contacts.h"

class IntersectionTests
{
public:
	static bool sphereAndHalfSpace(const ColliderSphere& sphere, const ColliderPlane& plane);
	static bool sphereAndSphere(const ColliderSphere& one, const ColliderSphere& two);
	static bool boxAndBox(const ColliderBox& one, const ColliderBox& two);
	static bool boxAndHalfSpace(const ColliderBox& box, const ColliderPlane& plane);
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
	static unsigned sphereAndHalfSpace(const ColliderSphere& sphere, const ColliderPlane& plane, CollisionData* data);
	static unsigned sphereAndTruePlane(const ColliderSphere& sphere, const ColliderPlane& plane, CollisionData* data);
	static unsigned sphereAndSphere(const ColliderSphere& one, const ColliderSphere& two, CollisionData* data);
	static unsigned boxAndHalfSpace(const ColliderBox& box, const ColliderPlane& plane, CollisionData* data);
	static unsigned boxAndBox(const ColliderBox& one, const ColliderBox& two, CollisionData* data);
	static unsigned boxAndPoint(const ColliderBox& box, const Vector3& point, CollisionData* data);
	static unsigned boxAndSphere(const ColliderBox& box, const ColliderSphere& sphere, CollisionData* data);
};

#endif