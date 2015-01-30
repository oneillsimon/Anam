#ifndef COLLISION_H
#define COLLISION_H

#include "PhysicsObject.h"

class CollisionTester
{
public:
	static bool sphereAndSphere(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data = 0);
	static bool boxAndBox(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data = 0);
	
	static void addCollisionImpulse(PhysicsObject& p0, PhysicsObject& p1, CollisionData_& data);
};

#endif