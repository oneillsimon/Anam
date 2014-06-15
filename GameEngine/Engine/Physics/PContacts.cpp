#include "PContacts.h"

void ParticleContact::resolveVelocity(float duration)
{
	// Find the velocity in the direction of the contact
	float separatingVelocity = calculateSeparatingVelocity();

	// Chech whether it needs to be resolved
	if(separatingVelocity > 0)
	{
		// The contact is either separating or stationary, no impulse needed
		return;
	}

	// Calculate the new separating velocity
	float newSepVelocity = -separatingVelocity * restitution;
	float deltaVelocity = newSepVelocity - separatingVelocity;

	// We apply the change in velocity to each in proportion to its inverse mass
	float totalInverseMass = particle[0]->getInverseMass();

	if(particle[1])
	{
		totalInverseMass += particle[1]->getInverseMass();
	}

	// If all particles have infinite mass then impulses have no effect
	if(totalInverseMass <= 0)
	{
		return;
	}

	// Calculate the impulse to apply
	float impulse = deltaVelocity / totalInverseMass;

	// Find the amount of impulse per unit of inverse mass
	Vector3 impulsePerInvMass = contactNormal * impulse;

	// Apply the impulses
	particle[0]->setVelocity(particle[0]->getVelocity() +
							 impulsePerInvMass * particle[0]->getInverseMass());

	if(particle[1])
	{
		// Particle 1 goes in the opposite direction
		particle[1]->setVelocity(particle[1]->getVelocity() +
								 impulsePerInvMass * -particle[1]->getInverseMass());
	}
}

void ParticleContact::resolve(float duration)
{
	resolveVelocity(duration);
}

float ParticleContact::calculateSeparatingVelocity() const
{
	Vector3 relativeVelocity  = particle[0]->getVelocity();

	if(particle[1])
	{
		relativeVelocity -= particle[1]->getVelocity();
	}

	return (relativeVelocity.getX() * relativeVelocity.getY() * relativeVelocity.getZ()) *
		   (contactNormal.getX() * contactNormal.getY() * contactNormal.getZ());
}