#include "RigidBody.h"

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
	if(!m_isAwake)
	{
		return;
	}

	m_lastFrameAcceleration = m_acceleration;
	m_lastFrameAcceleration.addScaledVector(m_forceAccum, m_inverseMass);

	Vector3 angularAcceleration = m_inverseInertiaTensorWorld.transform(m_torqueAccum);

	m_velocity.addScaledVector(m_lastFrameAcceleration, delta);
	m_rotation.addScaledVector(angularAcceleration, delta);

	m_velocity *= powf(m_linearDamping, delta);
	m_rotation *= powf(m_angularDamping, delta);

	m_parent->getTransform()->setPosition(m_parent->getTransform()->getPosition() + (m_velocity * delta));
	m_parent->getTransform()->setRotation(m_parent->getTransform()->getRotation() + (m_rotation * delta));

	calculateDerivedData();

	clearAccumulators();

	if(m_canSleep)
	{
		float currentMotion = m_velocity.scalarProduct(m_velocity) + m_rotation.scalarProduct(m_rotation);
		float bias = powf(0.5, delta);

		m_motion = bias*m_motion + (1 - bias)*currentMotion;

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

void RigidBody::calculateDerivedData()
{
	m_inverseInertiaTensorWorld = m_inverseInertiaTensor * m_parent->getTransform()->getTransformation().toMatrix3();
}

void RigidBody::setParent(PhysicsComponent* physicsObject)
{
	m_parent = physicsObject;
}

PhysicsComponent* RigidBody::getParent()
{
	return m_parent;
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
	m_parent->getTransform()->setPosition(position);
}

Vector3 RigidBody::getPosition() const
{
	return m_parent->getTransform()->getPosition();
}

void RigidBody::setOrientation(Quaternion& orientation)
{
	m_parent->getTransform()->setRotation(orientation.normalised());
}

Quaternion RigidBody::getOrientation() const
{
	return m_parent->getTransform()->getRotation();
}

Vector3 RigidBody::getPointInLocalSpace(const Vector3& point) const
{
	return m_parent->getTransform()->getTransformation().transformInverse(point);
}

Vector3 RigidBody::getPointInWorldSpace(const Vector3& point) const
{
	return m_parent->getTransform()->getTransformation().transform(point);
}

Vector3 RigidBody::getDirectionInLocalSpace(const Vector3& direction) const
{
	return m_parent->getTransform()->getTransformation().transformInverseDirection(direction);
}

Vector3 RigidBody::getDirectionInWorldSpace(const Vector3& direction) const
{
	return m_parent->getTransform()->getTransformation().transformDirection(direction);
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
	if(deltaVelocity.length() > 1000)
	{
		int b = 0;
	}

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

bool RigidBody::getAwake() const
{
	return m_isAwake;
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
		m_velocity = Vector3();
		m_rotation = Vector3();
	}
}

bool RigidBody::getCanSleep() const
{
	return m_canSleep;
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
	m_forceAccum = Vector3();
	m_torqueAccum = Vector3();
}

void RigidBody::addForce(const Vector3& force)
{
	m_isAwake = true;
	m_forceAccum += force;
}

void RigidBody::addForceAtBodyPoint(const Vector3& force, const Vector3& point)
{
	Vector3 pt = getPointInWorldSpace(point);
	addForceAtPoint(force, pt);
}

void RigidBody::addForceAtPoint(const Vector3& force, const Vector3& point)
{
	Vector3 pt = point;
	pt -= m_parent->getTransform()->getPosition();

	m_forceAccum += force;
	m_torqueAccum += pt % force;

	m_isAwake = true;
}

void RigidBody::addTorque(const Vector3& torque)
{
	m_isAwake = true;
	m_torqueAccum += torque;
}

void RigidBody::setAcceleration(const Vector3& acceleration)
{
	m_acceleration = acceleration;
}

Vector3 RigidBody::getAcceleration() const
{
	return m_acceleration;
}