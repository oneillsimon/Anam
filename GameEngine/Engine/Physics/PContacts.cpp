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

	// Check the velocity build-up due to acceleration only
	Vector3 accCauseVelocity = particle[0]->getAcceleration();

	if(particle[1])
	{
		accCauseVelocity -= particle[1]->getAcceleration();
	}

	float accCauseSepVelocity = (accCauseVelocity.getX() * accCauseVelocity.getY() * accCauseVelocity.getZ()) *
								(contactNormal.getX() * contactNormal.getY() * contactNormal.getZ()) *
								duration;

	// If we've got a closing velocity due to acceleration build-up, remove it from the seperation velocity
	if(accCauseSepVelocity < 0)
	{
		newSepVelocity += restitution * accCauseSepVelocity;

		// Make sure the seperating velocity isn't negative
		if(newSepVelocity < 0)
		{
			newSepVelocity = 0;
		}
	}

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

void ParticleContact::resolveInterpenetration(float duration)
{
	// If there is no penetration, return
	if(penetration <= 0)
	{
		return;
	}

	// The movement of each object is based on their inverse mass, so total that
	float totalInverseMass = particle[0]->getInverseMass();

	if(particle[1])
	{
		totalInverseMass += particle[1]->getInverseMass();
	}

	// If particles have infinite mass, return
	if(totalInverseMass <= 0)
	{
		return;
	}

	// Find amount of resolution in terms of inverse mass
	Vector3 movePerIMass = contactNormal * (-penetration / totalInverseMass);

	// Apply the resolution
	particle[0]->getTransform().setPosition(particle[0]->getTransform().getPosition() +
											movePerIMass *
											particle[0]->getInverseMass());

	if(particle[1])
	{
		particle[1]->getTransform().setPosition(particle[1]->getTransform().getPosition() +
												movePerIMass *
												particle[1]->getInverseMass());
	}
}

void ParticleContact::resolve(float duration)
{
	resolveVelocity(duration);
	resolveInterpenetration(duration);
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

ParticleContactResolver::ParticleContactResolver(unsigned iterations)
{
	this->iterations = iterations;
}

void ParticleContactResolver::resolveContacts(ParticleContact* contactArray, unsigned numberOfContacts, float duration)
{
	iterationsUsed = 0;

	while(iterationsUsed < iterations)
	{
		// Find the contact with the largest closing velocity
		float max = 0;
		unsigned maxIndex = numberOfContacts;

		for(int i = 0 ; i < numberOfContacts; i++)
		{
			float sepVelocity = contactArray[i].calculateSeparatingVelocity();

			if(sepVelocity < max)
			{
				max = sepVelocity;
				maxIndex = i;
			}
		}

		// Resolve this contact
		contactArray[maxIndex].resolve(duration);
		iterationsUsed ++;
	}
}

void ParticleContactResolver::setIterations(unsigned iterations)
{
	this->iterations = iterations;
}