#ifndef COLLIDEFINE_H
#define COLLIDEFINE_H

#include "Contacts.h"
#include "RigidBody.h"

class Primitive;

class CollisionData
{
public:
	Contact* m_contacts;
	unsigned m_contactsLeft;

	void detectContacts(const Primitive& firstPrimitive, const Primitive& secondPrimitive, CollisionData* data);
};

class CollisionDetector
{
public:
	unsigned sphereAndSphere(const Sphere_& one, const Sphere_& two, CollisionData* data);
	unsigned sphereAndHalfSapce(const Sphere_& sphere, const Plane_& plane, CollisionData* data);
	unsigned sphereAndTruePlane(const Sphere_& sphere, const Plane_& plane, CollisionData* data);
	unsigned boxAndSphere(const Box_& box, const Sphere_& sphere, CollisionData* data);
	unsigned boxAndPoint(const Box_& box, const Vector3& point, CollisionData* data);

	float transformToAxis(const Box_& box, const Vector3& axis);
	bool overlaOnAxis(const Box_& one, const Box_& two, const Vector3& axis);
};

class Primitive
{
public:
	RigidBody* m_body;
	Matrix4 m_offset;
	Matrix4 m_transform;

	Vector3 getAxis(unsigned int index) const;
};

class Sphere_ : public Primitive
{
public:
	float m_radius;
};

class Plane_ : public Primitive
{
public:
	Vector3 m_direction;
	float m_offset;
};

class Box_ : public Primitive
{
public:
	Vector3 m_halfSize;
}

#endif