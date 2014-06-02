#include <assert.h>
#include <math.h>
#include "Particle.h"

void Particle::update(float delta)
{
	assert(delta > 0);

	// Update position
	position.addScaledVector3(velocity, delta);

	// Determine acceleration from force
	Vector3 resultingAccel = acceleration;
	resultingAccel.addScaledVector3(forceAccumulated, inverseMass);
	
	// Update velocity
	velocity.addScaledVector3(resultingAccel, delta);

	// Apply damping
	velocity *= powf(damping, delta);
}