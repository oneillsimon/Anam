#ifndef COLLISION_H
#define COLLISION_H

#include <assert.h>

#include "Collider.h"
#include "Contacts.h"

class IntersectionTests
{
public:
	static bool colliderAndCollider(const Collider& one, const Collider& two);
	static bool sphereAndHalfSpace(const ColliderSphere& sphere, const ColliderPlane& plane);
	static bool sphereAndSphere(const ColliderSphere& one, const ColliderSphere& two);
	static bool boxAndBox(const ColliderBox& one, const ColliderBox& two);
	static bool boxAndHalfSpace(const ColliderBox& box, const ColliderPlane& plane);
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

class CollisionData
{
private:
	Contact* m_contactArray;
	Contact* m_contacts;
	int m_contactsLeft;
	unsigned m_contactCount;
	float m_friction;
	float m_restitution;
	float m_tolerance;

public:
	bool hasMoreContacts();
	void reset(unsigned maxContacts);
	void addContacts(unsigned count);

	Contact* getContactArray();
	Contact* getContacts();
	int getContactsLeft();
	unsigned getContactCount();

	float getFriction();
	float getRestitution();
	float getTolerance();

	void setContactArray(Contact* contactArray);

	void setFriction(float friction);
	void setRestitution(float restitution);
	void setTolerance(float tolerance);
};

#endif