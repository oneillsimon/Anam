#include "Particle.h"

class ParticleContact
{
private:

	// Handles the impulse calculations for this collision
	void resolveVelocity(float duration);

	// Holds the depth of penetration at the contact
	float penetration;

protected:

	// Resolves this contact
	void resolve(float duration);

	// Calculates the seperating velocity at this contact
	float calculateSeparatingVelocity() const;

public:

	// Holds the two particles in contact
	Particle* particle[2];

	// Holds the normal restitution at the contact
	float restitution;

	// Holds the direction of the contact in world coordinates
	Vector3 contactNormal;
};