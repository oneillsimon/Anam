#include "PFGenerator.h"

void ParticleForceRegistry::updateForces(float duration)
{
	Registry::iterator i = registrations.begin();

	for(; i != registrations.end(); i++)
	{
		i->particleForceGenerator->updateForce(i->particle, duration);
	}
}

ParticleGravity::ParticleGravity(const Vector3& gravity)
{
	this->gravity = gravity;
}

void ParticleGravity::updateForce(Particle* particle, float duration)
{
	if(!particle->hasFiniteMass())
	{
		return;
	}

	particle->addForce(gravity * particle->getMass());
}

ParticleDrag::ParticleDrag(float k1, float k2)
{
	this->k1 = k1;
	this->k2 = k2;
}

void ParticleDrag::updateForce(Particle* particle, float duration)
{
	Vector3 force;
	force = particle->getVelocity();

	// Calculate total drag coefficient
	float dragCoeff = force.length();
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

	// Calculate the final force and apply it
	force = force.normalised();
	force *= -dragCoeff;
	particle->addForce(force);
}

ParticleSpring::ParticleSpring(Particle* other, float springConstant, float restLength)
{
	this->other = other;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void ParticleSpring::updateForce(Particle* particle, float duration)
{
	// Calculate Vector3 of spring
	Vector3 force;
	force = particle->getTransform().getPosition();
	force -= other->getTransform().getPosition();

	// Calculate the magnitude of the force
	float magnitude = force.length();
	magnitude = fabsf(magnitude - restLength);
	magnitude *= springConstant;

	// Calculate the final force and apply it
	force = force.normalised();
	force *= -magnitude;
	particle->addForce(force);
}

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3* anchor, float springConstant, float restLength)
{
	this->anchor = anchor;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void ParticleAnchoredSpring::updateForce(Particle* particle, float duration)
{
	// Calculate Vector3 of spring
	Vector3 force;
	force = particle->getTransform().getPosition();
	force -= *anchor;

	// Calculate the magnitude of the force
	float magnitude = force.length();
	magnitude = abs(magnitude - restLength);
	magnitude *= springConstant;

	// Calculate the final force and apply it
	force = force.normalised();
	force *= -magnitude;
	particle->addForce(force);
}

ParticleBungee::ParticleBungee(Particle* other, float sprintConstant, float restLength)
{
	this->other = other;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void ParticleBungee::updateForce(Particle* particle, float duration)
{
	// Calculate the Vector3 of the spring
	Vector3 force;
	force = particle->getTransform().getPosition();
	force -= other->getTransform().getPosition();

	// Check if the bungee is compressed
	float magnitude = force.length();

	if(magnitude <= restLength)
	{
		return;
	}

	// Calculate the maginitude of the force
	magnitude = springConstant * (restLength - magnitude);

	// Calculate the final force and apply it
	force = force.normalised();
	force *= -magnitude;
	particle->addForce(force);
}

ParticleAnchoredBungee::ParticleAnchoredBungee(Vector3* anchor, float sprintConstant, float restLength)
{
	this->anchor = anchor;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void ParticleAnchoredBungee::updateForce(Particle* particle, float duration)
{
	// Calculate the Vector3 of the spring
	Vector3 force;
	force = particle->getTransform().getPosition();
	force -= *anchor;

	// Check if the bungee is compressed
	float magnitude = force.length();

	if(magnitude <= restLength)
	{
		return;
	}

	// Calculate the maginitude of the force
	magnitude = springConstant * (restLength - magnitude);

	// Calculate the final force and apply it
	force = force.normalised();
	force *= -magnitude;
	particle->addForce(force);
}

ParticleBuoyancy::ParticleBuoyancy(float maxDepth, float volume, float waterHeight, float liquidDensity)
{
	this->maxDepth = maxDepth;
	this->volume = volume;
	this->waterHeight = waterHeight;
	this->liquidDensity = liquidDensity;
}

void ParticleBuoyancy::updateForce(Particle* particle, float duration)
{
	// Calculate the submersion depth
	float depth = particle->getTransform().getPosition().getY();

	// Check if we're out of water
	if(depth >= waterHeight + maxDepth)
	{
		return;
	}

	Vector3 force = Vector3::ZERO;

	// Check if we're at maximum dpeth
	if(depth <= waterHeight - maxDepth)
	{
		force.setY(liquidDensity * volume);
		particle->addForce(force);
		return;
	}

	// Otherwise we're partially submerged
	force.setY(liquidDensity *
			   volume *
			   (depth - maxDepth - waterHeight) / 2 *
			   maxDepth);
	particle->addForce(force);
}

ParticleFakeSpring::ParticleFakeSpring(Vector3* anchor, float springConstant, float damping)
{
	this->anchor = anchor;
	this->springConstant = springConstant;
	this->damping = damping;
}

void ParticleFakeSpring::updateForce(Particle* particle, float duration)
{
	// Check that the particle does not have infinte mass
	if(!particle->hasFiniteMass())
	{
		return;
	}

	// Calculate the relative position of the particle to the anchor
	Vector3 position;
	position = particle->getTransform().getPosition();
	position -= *anchor;

	// Calculate the constants and check whether they are in bounds
	float gamma = 0.5f * sqrtf(4 * springConstant - damping * damping);

	if(gamma == 0)
	{
		return;
	}

	Vector3 c = position * (damping / (2.0f * gamma)) + particle->getVelocity() * (1.0f / gamma);

	// Calculate the target position
	Vector3 target = position * cosf(gamma * duration) + c * sinf(gamma * duration);
	target *= expf(-0.5f * duration * damping);

	// Calculate the resulting acceleration and therefore the force
	Vector3 a = (target - position) * (1.0f / duration * duration) - particle->getVelocity() * duration;
	particle->addForce(a * particle->getMass());
}