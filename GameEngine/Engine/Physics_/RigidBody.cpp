#include "RigidBody.h"

void RigidBody::input(const Input& input, float delta)
{
	if(input.getKey(Input::KEY_P))
	{
		addLinearForce(Vector3(0, 0, 5));
	}

	if(input.getKey(Input::KEY_L))
	{
		addAngularForce(Vector3(50, 50, 50));
	}

	if(input.getKey(Input::KEY_K))
	{
		addAngularForce(Vector3(50, 0, 0));
	}

	if(input.getKey(Input::KEY_M))
	{
		addLinearForce(Vector3(3, 3, 3));
		addAngularForce(Vector3(3, 3, 3));
	}
}

void RigidBody::update(float delta)
{
	integrate(delta);
}

void RigidBody::integrate(float delta)
{
	m_acceleration += m_forceAccum * m_inverseMass;
	Vector3 angularAcceleration = m_torqueAccum * m_inverseMass;

	m_velocity += m_acceleration * delta;
	m_rotation += angularAcceleration * delta;

	m_velocity *= powf(m_linearDamping, delta);
	m_rotation *= powf(m_angularDamping, delta);

	getTransform()->setPosition(getTransform()->getPosition() + (m_velocity * delta));
	getTransform()->rotate(m_rotation, m_rotation.length());

	m_velocity *= powf(m_linearDamping, delta);
	m_rotation *= powf(m_angularDamping, delta);

	clearAccumulators();
}

void RigidBody::addLinearForce(const Vector3& force)
{
	m_forceAccum += force;
}

void RigidBody::addAngularForce(const Vector3& force)
{
	m_torqueAccum += force;
}

void RigidBody::addForceAtBodyPoint(const Vector3& force, const Vector3& point)
{
	Vector3 pnt = m_parent->getTransform()->getTransformedPoint(point);
}

void RigidBody::clearAccumulators()
{
	m_forceAccum = Vector3();
	m_torqueAccum = Vector3();
}

float RigidBody::getInverseMass() const
{
	return m_inverseMass;
}

float RigidBody::getMass()
{
	return 1.0f / m_inverseMass;
}