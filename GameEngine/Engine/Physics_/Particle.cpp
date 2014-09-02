#include <math.h>

#include "Particle.h"

void Particle::integrate(float delta)
{
	// Update positoion linearly
	m_position += m_velocity * delta;

	// Determine acceleration from force
	Vector3 resultingAccel = m_acceleration + (m_forceAccumulated * m_inverseMass);

	// Update linear velocity from the acceleration
	m_velocity += resultingAccel * delta;

	// Impose drag
	m_velocity *= powf(m_damping, delta);
}

Vector3 Particle::getPosition() const
{
	return m_position;
}

Vector3 Particle::getVelocity() const
{
	return m_velocity;
}

Vector3 Particle::getAcceleration() const
{
	return m_acceleration;
}

float Particle::getDamping()
{
	return m_damping;
}

float Particle::getInversMass()
{
	return m_inverseMass;
}

Vector3 Particle::getAccumulatedForce() const
{
	return m_forceAccumulated;
}

void Particle::setPosition(const Vector3& position)
{
	m_position = position;
}

void Particle::setVelocity(const Vector3& velocity)
{
	m_velocity = velocity;
}

void Particle::setAcceleration(const Vector3& acceleration)
{
	m_acceleration = acceleration;
}

void Particle::setDamping(float damping)
{
	m_damping = damping;
}

void Particle::setMass(float mass)
{
	m_inverseMass = 1.0f / mass;
}