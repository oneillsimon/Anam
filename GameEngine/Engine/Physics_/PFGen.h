#ifndef PFGEN_H
#define PFGEN_H

#include "Particle.h"
#include "RigidBody.h"

class ParticleForceGenerator
{
public:
	virtual void updateForce(Particle* particle, float delta) = 0;
};

class ParticleForceRegistry
{
protected:
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* forceGenerator;

		ParticleForceRegistration operator =(const ParticleForceRegistration& other)
		{
			particle = other.particle;
			forceGenerator = other.forceGenerator;
			return *this;
		};
	};

	std::vector<ParticleForceRegistration> m_registrations;

public:
	ParticleForceRegistry() {}
	ParticleForceRegistry(const ParticleForceRegistry& other);

	void add(Particle* particle, ParticleForceGenerator* forceGenerator);
	void remove(Particle* particle, ParticleForceGenerator* forceGenerator);
	void clear();
	void updateForces(float delta);

	Particle* getParticle(unsigned int index) const;
	std::vector<ParticleForceRegistration> getRegistrations() const;
	unsigned int getLength();

	ParticleForceRegistry operator =(const ParticleForceRegistry& other);
};

class ParticleGravity : public ParticleForceGenerator
{
private:
	Vector3 m_gravity;

public:
	ParticleGravity(const Vector3& gravity);

	virtual void updateForce(RigidBody* body, float delta);
};

class ParticleDrag : public ParticleForceGenerator
{
private:
	float m_k1;
	float m_k2;

public:
	ParticleDrag(float k1, float k2);

	virtual void updateForce(Particle* particle, float delta);
};

class ParticleSpring : public ParticleForceGenerator
{
private:
	Particle* m_other;
	
	float m_springConstant;
	float m_restLength;

public:
	ParticleSpring(Particle* other, float springConstant, float restLength);
	~ParticleSpring();

	virtual void updateForce(Particle* particle, float delta);
};

#endif