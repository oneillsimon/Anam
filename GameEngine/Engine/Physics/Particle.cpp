#include <assert.h>
#include <math.h>
#include "Particle.h"

Particle::Particle(Vector3 velocity, Vector3 acceleration, float mass, float damping)
{
	this->velocity = velocity;
	this->acceleration = acceleration;
	this->inverseMass = 1.0f / mass;
	this->damping = damping;
	this->forceAccumulated = Vector3::ZERO;
}

Particle::~Particle()
{
}

void Particle::update(float delta)
{
	Vector3 v = getTransform().getPosition();
	printf("%.4f, %.4f, %.4f\n", v.getX(), v.getY(), v.getZ());

	assert(delta > 0);

	// Update position
	getTransform().getPosition().addScaledVector3(velocity, delta);

	// Determine acceleration from force
	Vector3 resultingAccel = acceleration;
	resultingAccel.addScaledVector3(forceAccumulated, inverseMass);
	
	// Update velocity
	velocity.addScaledVector3(resultingAccel, delta);

	// Apply damping
	velocity *= powf(damping, delta);

	clearAccumulator();
}

void Particle::addForce(const Vector3& v)
{
	forceAccumulated += v;
}

void Particle::clearAccumulator()
{
	forceAccumulated = Vector3::ZERO;
}

bool Particle::hasFiniteMass()
{
	return inverseMass >= 0;
}

Vector3 Particle::getVelocity() const
{
	return velocity;
}

Vector3 Particle::getAcceleration() const
{
	return acceleration;
}

float Particle::getDamping() const
{
	return damping;
}

float Particle::getInverseMass() const
{
	return inverseMass;
}

Vector3 Particle::getForceAccumulated() const
{
	return forceAccumulated;
}

void Particle::setVelocity(const Vector3& v)
{
	velocity = v;
}

void Particle::setAcceleration(const Vector3& v)
{
	acceleration = v;
}

void Particle::setDamping(float f)
{
	damping = f;
}

void Particle::setMass(float mass)
{
	inverseMass = 1.0f / mass;
}