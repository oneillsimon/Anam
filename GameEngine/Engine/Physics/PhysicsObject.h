#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "../Core/GameObject.h"
#include "Collider.h"

#define LINEAR_VELOCITY_DAMP 0.998
#define ANGULAR_VELOCITY_DAMP 0.998
#define LINEAR_VELOCITY_MIN 0.00001

class PhysicsObject : public GameObject
{
private:
	Vector3 m_linearVelocity;
	float m_invMass;
	Matrix4 m_invInertia;
	Vector3 m_angualrVelocity;
	Collider* m_collider;
	Vector3 m_force;
	Vector3 m_torque;
	
public:

	PhysicsObject(const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(0, 0, 0, 1), float scale = 1.0f);

	Vector3 getLinearVelocity() const;
	float getInverseMass() const;
	float getMass() const;

	Matrix4 getInverseInertiaTensor() const;
	
	Vector3 getAngularVelocity() const;

	Collider* getCollider() const;

	Matrix4 buildTransform();

	void setLinearVelocity(const Vector3& v);
	void setMass(float mass);
	void setInverseMass(float f);

	void setInverseInertia(const Matrix4& m);
	void setAngularVelocity(const Vector3& v);

	void setCollider(Collider* c);

	Vector3 getForce();
	Vector3 getTorque();

	void addForce(const Vector3& point, const Vector3& force);

	virtual void initialise();
	virtual void update(float delta);
};

#endif