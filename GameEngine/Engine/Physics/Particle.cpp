#include <math.h>

#include "Particle.h"

Particle::Particle(const Shape& shape) :
	m_shape(shape)
{
}

Particle::Particle(const Particle& other) :
	m_acceleration(other.getAcceleration()),
	m_damping(other.getDamping()),
	m_forceAccumulated(other.getAccumulatedForce()),
	m_inverseMass(other.getInverseMass()),
	m_position(other.getPosition()),
	m_shape(other.getShape()),
	m_velocity(other.getVelocity())
{
}

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

	resetAcummulator();
}

void Particle::addForce(const Vector3& force)
{
	m_forceAccumulated += force;
}

void Particle::resetAcummulator() const
{
	m_forceAccumulated = Vector3();
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

float Particle::getDamping() const
{
	return m_damping;
}

float Particle::getInverseMass() const
{
	return m_inverseMass;
}

float Particle::getMass()
{
	return 1.0f / m_inverseMass;
}

Vector3 Particle::getAccumulatedForce() const
{
	return m_forceAccumulated;
}

Shape Particle::getShape() const
{
	return m_shape;
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

Particle Particle::operator =(const Particle& particle)
{
	m_acceleration = particle.getAcceleration();
	m_damping = particle.getDamping();
	m_forceAccumulated = particle.getAccumulatedForce();
	m_inverseMass = particle.getInverseMass();
	m_position = particle.getPosition();
	m_shape = particle.getShape();
	m_velocity = particle.getVelocity();
	return *this;
}