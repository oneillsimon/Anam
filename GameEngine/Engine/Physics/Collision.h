#ifndef COLLISION_H
#define COLLISION_H

#include "PhysicsObject.h"

struct CollisionData_
{
	PhysicsObject* body[2];
	float m_penetration;
	Vector3 m_normal;
	Vector3 m_point;

	float friction;
	float restitution;

	Matrix4 contactToWorld;
	Vector3 contactVelocity;
	float desiredDeltaVelocity;
	Vector3 relativeContactPosition[2];

	void calculateInterals(float delta);
	void calculateDesiredDeltaVelocity();
	void calculateContactBasis();

	void applyImpulse(const Vector3& impulse)
};

class CollisionTester
{
public:
	static bool sphereAndSphere(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data = 0);
	static bool planeAndSphere(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data = 0);
	static bool boxAndBox(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data = 0);
	static bool oBBAndOBB(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data = 0);
	
	static void addCollisionImpulse(PhysicsObject& p0, PhysicsObject& p1, CollisionData_& data);
};

class ContactResolver
{
private:
	bool m_validSettings;

protected:
	unsigned m_velocityIterations;
	unsigned m_positionIterations;
	float m_velocityEpsilon;
	float m_positionEpsilon;

	void prepareContacts(CollisionData_* data, unsigned numContacts, float delta);
	void adjustVelocities(CollisionData_* data, unsigned numContacts, float delta);
	void adjustPositions(CollisionData_* data, unsigned numContacts, float delta);

public:
	unsigned velocityIterationsUsed;
	unsigned positionIterationsUsed;

	ContactResolver(unsigned iterations, float velocityEpsilon, float positionEpislon);
	ContactResolver(unsigned velocityIterations, unsigned positionIterations,
					float velocityEpsilon = 0.01f, float positionEpsilon = 0.01f);

	bool isValid()
	{
		return (m_velocityIterations > 0) &&
			   (m_positionIterations > 0) &&
			   (m_positionEpsilon >= 0.0f) &&
			   (m_velocityEpsilon >= 0.0f);
	}

	void setIterations(unsigned velocitIterations,
		unsigned positionIterations);
	void setIterations(unsigned iterations);
	void setEpsilon(float velocityEpsilon, float positionEpsilon);

	void resolveContacts(CollisionData_* data, unsigned numContacts, float delta);
};

#endif