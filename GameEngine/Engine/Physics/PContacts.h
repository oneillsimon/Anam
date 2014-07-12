#include "Particle.h"

class ParticleContact
{
private:

	// Handles the impulse calculations for this collision
	void resolveVelocity(float duration);

	
	// Handles the interpenetration resolution for this contact
	void resolveInterpenetration(float duration);

public:

	// Holds the two particles in contact
	Particle* particle[2];

	// Holds the depth of penetration at the contact
	float penetration;

	// Holds the normal restitution at the contact
	float restitution;

	// Holds the direction of the contact in world coordinates
	Vector3 contactNormal;

	// Resolves this contact
	void resolve(float duration);

	// Calculates the seperating velocity at this contact
	float calculateSeparatingVelocity() const;
};

class ParticleContactResolver
{
protected:

	// Holds the number of iterations allowed
	unsigned iterations;

	// Holds the number of iterations actually used
	unsigned iterationsUsed;

public:
	ParticleContactResolver(unsigned iterations);

	// Resolves a set of contacts
	void resolveContacts(ParticleContact* contactArray, unsigned numerOfContacts, float duration);

	void setIterations(unsigned interations);
};