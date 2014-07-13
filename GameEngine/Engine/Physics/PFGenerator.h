#ifndef PFGENERATOR_H
#define PFGENERATOR_H

#include "Particle.h"

class ParticleForceGenerator
{
public:
	virtual void updateForce(Particle* particle, float duration) = 0;
};

/*
	Holds all the force generators and particles they apply to.
*/
class ParticleForceRegistry
{
protected:

	// Keeps track of one force generator and the particle it applies to
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* particleForceGenerator;
	};

	// Holds the list of registrations
	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;

public:

	// Registers the given ParticleForceGenerator and Particle
	void add(Particle* particle, ParticleForceGenerator* particleForceGenerator);

	// Removes the given ParticleForceGenerator and Particle
	void remove(Particle* particle, ParticleForceGenerator* particleForceGenerator);

	// Clears the registry
	void clear();
	void updateForces(float duration);
};

class ParticleGravity : public ParticleForceGenerator
{
	Vector3 gravity;

public:
	ParticleGravity(const Vector3& gravity);
	virtual void updateForce(Particle* particle, float duration);
};

class ParticleDrag : public ParticleForceGenerator
{
	// Holds the velocity drag coefficient
	float k1;

	// Holds the velocity squared drag coefficient
	float k2;

public:
	ParticleDrag(float k1, float k2);
	virtual void updateForce(Particle* particle, float duration);
};

class ParticleSpring : public ParticleForceGenerator
{
	Particle *other;
	 float springConstant;
	 float restLength;

public:
	ParticleSpring(Particle* other, float springConstant, float restLength);
	virtual void updateForce(Particle* particle, float duration);
};

class ParticleAnchoredSpring : public ParticleForceGenerator
{
	Vector3* anchor;
	float springConstant;
	float restLength;

public:
	ParticleAnchoredSpring(Vector3* anchor, float springConstant, float restLength);
	virtual void updateForce(Particle* particle, float duration);
};

class ParticleBungee : public ParticleForceGenerator
{
	Particle* other;
	float springConstant;
	float restLength;

public:
	ParticleBungee(Particle* other, float springConstant, float restLength);
	virtual void updateForce(Particle* particle, float duration);
};

class ParticleAnchoredBungee : public ParticleForceGenerator
{
	Vector3* anchor;
	float springConstant;
	float restLength;

public:
	ParticleAnchoredBungee(Vector3* anchor, float springConstant, float restLength);
	virtual void updateForce(Particle* particle, float duration);
};

class ParticleBuoyancy : public ParticleForceGenerator
{
	// The maximum submersion depth of the object before it generates its maximum buoyancy force
	float maxDepth;

	// The volume of the object
	float volume;

	// The height of the watet plane above Y = 0;
	float waterHeight;

	// The density of the liquid, pure water has a density of 1000 kg/m^3
	float liquidDensity;

public:
	ParticleBuoyancy(float maxDepth, float volume, float waterHeight, float liquidDensity = 1000.0f);
	virtual void updateForce(Particle* particle, float duration);
};

class ParticleFakeSpring : public ParticleForceGenerator
{
	// The location of the anchored end of the spring
	Vector3* anchor;
	float springConstant;
	float damping;

public:
	ParticleFakeSpring(Vector3* anchor, float springConstant, float damping);
	virtual void updateForce(Particle* particle, float duration);
};

#endif