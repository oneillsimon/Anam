#include <assert.h>

#include "RigidBody.h"

void RigidBody::calculateDerivedData()
{
	m_owner->setRotation(m_owner->getRotation().normalised());

	// TODO: calculate inertiaTensor in world space.
}

RigidBody::RigidBody(float mass, float linear, float angular) :
m_linearDamping(linear),
m_angularDamping(angular)
{
	if(mass == -1)
	{
		m_hasInfiniteMass = true;
	}
	else
	{
		m_hasInfiniteMass = false;
	}

	m_inverseMass = 1.0f / mass;
}

void RigidBody::integrate(float delta)
{
	if(!m_isAwake || m_hasInfiniteMass)
	{
		return;
	}

	addForce(Vector3(0, -9.8f, 0) * delta);
	
	m_lastFrameAcceleration = m_acceleration;
	m_lastFrameAcceleration.addScaledVector3(m_forceAccum, m_inverseMass);

	Vector3 angularAcceleration = m_inverseInertiaTensorWorld.transform(m_torqueAccum);

	m_velocity.addScaledVector3(m_lastFrameAcceleration, delta);
	m_rotation.addScaledVector3(angularAcceleration, delta);

	m_velocity *= powf(m_linearDamping, delta);
	m_rotation *= powf(m_angularDamping, delta);

	m_owner->setPosition(m_owner->getPosition() + (m_velocity * delta));
	//m_owner->rotate(AXIS_Z, m_rotation.length() + 0.00001f);

	calculateDerivedData();
	clearAccumulators();

	if(m_canSleep)
	{
		float currentMotion = m_velocity.scalarProduct(m_velocity) + m_rotation.scalarProduct(m_rotation);

		float bias = powf(0.5f, delta);
		m_motion = bias * m_motion + (1 - bias) * currentMotion;

		if(m_motion < m_sleepEpsilon)
		{
			setAwake(false);
		}
		else if(m_motion > 10 * m_sleepEpsilon)
		{
			m_motion = 10 * m_sleepEpsilon;
		}
	}
}

void RigidBody::setMass(float mass)
{
	assert(mass != 0);
	m_inverseMass = 1.0f / mass;
}

float RigidBody::getMass() const
{
	if(m_inverseMass == 0)
	{
		return FLT_MAX;
	}
	else
	{
		return 1.0f / m_inverseMass;
	}
}

void RigidBody::setInverseMass(const float inverseMass)
{
	m_inverseMass = inverseMass;
}
float RigidBody::getInverseMass() const
{
	return m_inverseMass;
}

bool RigidBody::hasFiniteMass() const
{
	return m_inverseMass >= 0.0f;
}

void RigidBody::setIntertiaTensor(const Matrix3& intertiaTensor)
{
	m_inverseInertiaTensor.setInverse(intertiaTensor);
}

void RigidBody::getInertiaTensor(Matrix3* intertiaTensor) const
{
	intertiaTensor->setInverse(m_inverseInertiaTensor);
}

Matrix3 RigidBody::getInertiaTensor() const
{
	Matrix3 it;
	getInertiaTensor(&it);
	return it;
}

void RigidBody::getInertiaTensorWorld(Matrix3* interiaTensor) const
{
	interiaTensor->setInverse(m_inverseInertiaTensorWorld);
}

Matrix3 RigidBody::getInertiaTensorWorld() const
{
	Matrix3 it;
	getInertiaTensorWorld(&it);
	return it;
}

void RigidBody::setInverseInertiaTensor(const Matrix3& inverseInertiaTensor)
{
	m_inverseInertiaTensor = inverseInertiaTensor;
}

Matrix3 RigidBody::getInverseInertiaTensor() const
{
	return m_inverseInertiaTensor;
}

Matrix3 RigidBody::getInverseInertiaTensorWorld() const
{
	return m_inverseInertiaTensorWorld;
}

void RigidBody::setDamping(const float linearDamping, const float angularDamping)
{
	m_linearDamping = linearDamping;
	m_angularDamping = angularDamping;
}

void RigidBody::setLinearDamping(const float linearDamping)
{
	m_linearDamping = linearDamping;
}

float RigidBody::getLinearDamping() const
{
	return m_linearDamping;
}

void RigidBody::setAngularDamping(const float angularDamping)
{
	m_angularDamping = angularDamping;
}

float RigidBody::getAngularDamping() const
{
	return m_angularDamping;
}

void RigidBody::setPosition(const Vector3& position)
{
	m_owner->setPosition(position);
}

Vector3 RigidBody::getPosition() const
{
	return m_owner->getPosition();
}

void RigidBody::setOrientation(Quaternion& orientation)
{
	m_owner->setRotation(orientation.normalised());
}

Quaternion RigidBody::getOrientation() const
{
	return m_owner->getRotation();
}

Vector3 RigidBody::getPointInLocalSpace(const Vector3& point) const
{
	return m_owner->getTransformation().transformInverse(point);
}

Vector3 RigidBody::getPointInWorldSpace(const Vector3& point) const
{
	return m_owner->getTransformation().transform(point);
}

Vector3 RigidBody::getDirectionInLocalSpace(const Vector3& direction) const
{
	return m_owner->getTransformation().transformInverseDirection(direction);
}

Vector3 RigidBody::getDirectionInWorldSpace(const Vector3& direction) const
{
	return m_owner->getTransformation().transformDirection(direction);
}

void RigidBody::setVelocity(const Vector3& velocity)
{
	m_velocity = velocity;
}

Vector3 RigidBody::getVelocity() const
{
	return m_velocity;
}

void RigidBody::addVelocity(const Vector3& deltaVelocity)
{
	m_velocity += deltaVelocity;
}

void RigidBody::setRotation(const Vector3& rotation)
{
	m_rotation = rotation;
}

Vector3 RigidBody::getRotation() const
{
	return m_rotation;
}

void RigidBody::addRotation(const Vector3& deltaRotation)
{
	m_rotation += deltaRotation;
}

void RigidBody::setAwake(const bool awake)
{
	if(awake)
	{
		m_isAwake = true;
		m_motion = m_sleepEpsilon * 2.0f;
	}
	else
	{
		m_isAwake = false;
		m_velocity.clear();
		m_rotation.clear();
	}
}

void RigidBody::setCanSleep(const bool canSleep)
{
	m_canSleep = canSleep;

	if(!canSleep && !m_isAwake)
	{
		setAwake();
	}
}

Vector3 RigidBody::getLastFrameAcceleration() const
{
	return m_lastFrameAcceleration;
}

void RigidBody::clearAccumulators()
{
	m_forceAccum.clear();
	m_torqueAccum.clear();
}

void RigidBody::addForce(const Vector3& force)
{
	m_forceAccum += force;
	m_isAwake = true;
}

void RigidBody::addForceAtBodyPoint(const Vector3& force, const Vector3& point)
{
	Vector3 pt = getPointInWorldSpace(point);
	addForceAtPoint(force, pt);
}

void RigidBody::addForceAtPoint(const Vector3& force, const Vector3& point)
{
	Vector3 pt = point;
	pt -= m_owner->getPosition();

	m_forceAccum += force;
	m_torqueAccum += pt % force;

	m_isAwake = true;
}

void RigidBody::addTorque(const Vector3& torque)
{
	m_torqueAccum += torque;
	m_isAwake = true;
}

void RigidBody::setAcceleration(const Vector3& acceleration)
{
	m_acceleration = acceleration;
}

Vector3 RigidBody::getAcceleration() const
{
	return m_acceleration;
}