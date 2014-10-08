#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Core/GameComponent.h"
#include "../Core/Math3D.h"

class RigidBody : public GameComponent
{
public:
	float m_inverseMass;
	Vector3 m_velocity;
	Vector3 m_acceleration;
	Vector3 m_rotation;
	float m_linearDamping;
	float m_angularDamping;

	Vector3 m_forceAccum;
	Vector3 m_torqueAccum;

	virtual void input(const Input& input, float delta);
	virtual void update(float delta);

	void integrate(float delta);

	void addLinearForce(const Vector3& force);
	void addAngularForce(const Vector3& force);
	void addForceAtBodyPoint(const Vector3& force, const Vector3& point);
	void clearAccumulators();

	float getInverseMass() const;
	float getMass();
};

#endif