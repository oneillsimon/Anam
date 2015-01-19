#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Core/GameComponent.h"
#include "../Core/Math3D.h"

class RigidBody : public GameComponent
{
public:
	const float m_sleepEpsilon = 0.01f;

	float m_inverseMass;
	Matrix3 m_inverseInertiaTensor;
	float m_linearDamping;
	float m_angularDamping;
	Vector3 m_velocity;
	Vector3 m_acceleration;
	Vector3 m_rotation;
	Matrix3 m_inverseInertiaTensorWorld;
	float m_motion;
	bool m_isAwake;
	bool m_canSleep;
	//Matrix4 m_transformMatrix;
	bool m_hasInfiniteMass;

	Vector3 m_forceAccum;
	Vector3 m_torqueAccum;
	Vector3 m_lastFrameAcceleration;

	RigidBody(float mass, float linear = 1, float angular = 2);

	virtual void input(const Input& input, float delta);
	virtual void update(float delta);

	void calculateDerivedData();
	void integrate(float delta);

	void clearAccumulators();

	bool hasFiniteMass() const;

	void setMass(const float mass);
	void setInverseMass(const float inverseMass);
	void setIntertiaTensor(const Matrix3& interiaTensor);
	void setInverseInertiaTensor(const Matrix3& inverseInertiaTensor);
	void setDamping(const float linearDamping, const float angularDamping);
	void setLinearDamping(const float linearDamping);
	void setAngularDamping(const float angularDamping);
	void setPosition(const Vector3& position);
	void setOrientation(Quaternion& orientation);

	float getInverseMass() const;
	float getMass() const;
	void getInertiaTensor(Matrix3* intertiaTensor) const;
	Matrix3 getInertiaTensor() const;
	void getInertiaTensorWorld(Matrix3* intertiaTensor) const;
	Matrix3 getInertiaTensorWorld() const;
	Matrix3 getInverseInertiaTensor() const;
	Matrix3 getInverseInertiaTensorWorld() const;
	float getLinearDamping() const;
	float getAngularDamping() const;
	Vector3 getPosition() const;
	Quaternion getOrientation() const;

	Vector3 getPointInLocalSpace(const Vector3& point) const;
	Vector3 getPointInWorldSpace(const Vector3& point) const;
	Vector3 getDirectionInLocalSpace(const Vector3& direction) const;
	Vector3 getDirectionInWorldSpace(const Vector3& direction) const;

	void setVelocity(const Vector3& velocity);
	Vector3 getVelocity() const;
	void addVelocity(const Vector3& deltaVelocity);

	void setRotation(const Vector3& rotation);
	Vector3 getRotation() const;
	void addRotation(const Vector3& deltaRotation);

	bool getAwake() const
	{
		return m_isAwake;
	}

	void setAwake(const bool awake = true);

	bool getCanSleep() const
	{
		return m_canSleep;
	}

	void setCanSleep(const bool canSleep = true);

	Vector3 getLastFrameAcceleration() const;

	void addForce(const Vector3& force);
	void addForceAtPoint(const Vector3& force, const Vector3& point);
	void addForceAtBodyPoint(const Vector3& force, const Vector3& point);
	void addTorque(const Vector3& torque);

	void setAcceleration(const Vector3& acceleration);
	Vector3 getAcceleration() const;


};

#endif