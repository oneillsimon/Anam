#ifndef CONTACTS_H
#define CONTACTS_H

#include <math.h>

#include "../Core/Math3D.h"
#include "RigidBody.h"

class ContactResolver;

class Contact
{
private:
	RigidBody* m_body[2];
	float m_friction;
	float m_restitution;
	Vector3 m_contactPoint;
	Vector3 m_contactNormal;
	float m_penetration;

	Matrix3 m_contactToWorld;
	Vector3 m_contactVelocity;
	float m_desiredDeltaVelocity;
	Vector3 m_relativeContactPosition[2];

public:
	void setBodyData(RigidBody* one, RigidBody* two, float friction, float restitution);
	void calculateInternals(float duration);

	void swapBodies();
	void matchAwakeState();
	void calculateDesiredDeltaVelocity(float duration);
	Vector3 calculateLocalVelocity(unsigned bodyIndex, float duration);
	void calculateContactBasis();
	void applyImpulse(const Vector3& impulse, RigidBody* body, Vector3* veloctyChange, Vector3* rotationChange);
	void applyVelocityChange(Vector3 velocityChange[2], Vector3 angularChange[2]);
	void applyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], float penetration);
	Vector3 calculateFrictionlessImpulse(Matrix3* inverseIntertiaTensor);
	Vector3 calculateFrictionImpulse(Matrix3* inverseIntertiaTensor);

	void addContactVelocity(const Vector3& deltaVelocity);
	void addPenetration(float deltaPenetration);

	RigidBody* getBody(unsigned index);
	float getFriction();
	float getRestitution();
	Vector3 getContactPoint() const;
	Vector3 getContactNormal() const;
	float getPenetration();
	Matrix3 getContactToWorld() const;
	Vector3 getContactVelocity() const;
	float getDesiredDeltaVelocity();
	Vector3 getRelativeContactPosition(unsigned index);
	void setBody(RigidBody* body, unsigned index);
	void setFriction(float friction);
	void setRestitution(float restitution);
	void setContactPoint(const Vector3& contactPoint);
	void setContactNormal(const Vector3& contactNormal);
	void setPenetration(float penetration);
	void setContactToWorld(const Matrix3& contactToWorld);
	void setContactVelocity(const Vector3& contactVelocity);
	void setDesiredDeltaVelocity(float desiredDeltaVelocity);
	void setRelativeContactPosition(const Vector3& contactPosition, unsigned index);
};

class ContactResolver
{
protected:
	unsigned m_velocityIterations;
	unsigned m_positionIterations;
	float m_velocityEpsilon;
	float m_positionEpsilon;

public:
	unsigned m_velocityIterationsUsed;
	unsigned m_positionIterationsUsed;

private:
	bool m_validSettings;

public:
	ContactResolver(unsigned iterations, float velocityEpsilon = 0.01f, float positionEpsilon = 0.01f);
	ContactResolver(unsigned velocityIterations, unsigned positionIterations, float velocityEpsilon = 0.01f, float positionEpsilon = 0.01f);

	bool isValid();
	void setIterations(unsigned velocityIterations, unsigned positionIterations);
	void setIterations(unsigned iterations);
	void setEpsilon(float velocityEpsilon, float positionEpsilon);
	void resolveContacts(Contact* contactArray, unsigned numContacts, float duration);

protected:
	void prepareContacts(Contact* contactArray, unsigned numContacts, float duration);
	void adjustVelocities(Contact* contactArray, unsigned numContacts, float duration);
	void adjustPositions(Contact* contacts, unsigned numContacts, float duration);
};

class ContactGenerator
{
public:
	virtual unsigned addContact(Contact* contact, unsigned limit) const = 0;
};

#endif