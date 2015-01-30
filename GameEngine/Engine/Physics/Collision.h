#ifndef COLLISION_H
#define COLLISION_H

#include "PhysicsObject.h"

struct CollisionData_
{
	float m_penetration;
	Vector3 m_normal;
	Vector3 m_point;
};

class CollisionTester
{
public:
	static bool sphereAndSphere(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data = 0);
	static bool boxAndBox(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data = 0);
	
	static void addCollisionImpulse(PhysicsObject& p0, PhysicsObject& p1, CollisionData_& data);
};

//float lengthSquared(const Vector3& v)
//{
//	return v.dot(v);
//}

#endif