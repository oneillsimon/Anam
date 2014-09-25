#include "PFGen.h"

ParticleForceRegistry::ParticleForceRegistry(const ParticleForceRegistry& other)
{
	for(unsigned int i = 0; i < other.m_registrations.size(); i++)
	{
		ParticleForceRegistration registration = ParticleForceRegistration();
		malloc(sizeof(Particle));
		memcpy(registration.particle, other.m_registrations[i].particle, sizeof(Particle));

		malloc(sizeof(ParticleForceGenerator));
		memcpy(registration.forceGenerator, other.m_registrations[i].forceGenerator, sizeof(ParticleForceGenerator));
		//registration.particle = other.m_registrations[i].particle;
		//registration.forceGenerator = other.m_registrations[i].forceGenerator;
		m_registrations.push_back(registration);
	}
}

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* forceGenerator)
{
	ParticleForceRegistration registration;
	registration.particle = particle;
	registration.forceGenerator = forceGenerator;
	m_registrations.push_back(registration);
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* forceGenerator)
{
	// TODO: proper remove
	m_registrations.pop_back();
}

void ParticleForceRegistry::clear()
{
	m_registrations.clear();
}

void ParticleForceRegistry::updateForces(float delta)
{
	/*std::vector<ParticleForceRegistration>::iterator i = m_registrations.begin();

	for(; i != m_registrations.end(); i++)
	{
		i->forceGenerator->updateForce(i->particle, delta);
	}*/
}

Particle* ParticleForceRegistry::getParticle(unsigned int index) const
{
	return m_registrations[index].particle;
}

std::vector<ParticleForceRegistry::ParticleForceRegistration> ParticleForceRegistry::getRegistrations() const
{
	return m_registrations;
}

unsigned int ParticleForceRegistry::getLength()
{
	return m_registrations.size();
}

ParticleForceRegistry ParticleForceRegistry::operator =(const ParticleForceRegistry& other)
{
	m_registrations = other.m_registrations;
	return *this;
}

ParticleGravity::ParticleGravity(const Vector3& gravity)
{
	m_gravity = gravity;
}

void ParticleGravity::updateForce(RigidBody* body, float delta)
{
	// TODO: check for infinite mass

	body->addLinearForce(m_gravity * body->getMass());
}

ParticleDrag::ParticleDrag(float k1, float k2) :
	m_k1(k1),
	m_k2(k2)
{
}

void ParticleDrag::updateForce(Particle* particle, float delta)
{
	Vector3 force = particle->getVelocity();

	float dragCoefficient = force.length();
	dragCoefficient = m_k1 * dragCoefficient + m_k2 * dragCoefficient * dragCoefficient;

	force = force.normalised();
	force *= -dragCoefficient;
	particle->addForce(force);
}

ParticleSpring::ParticleSpring(Particle* other, float springConstant, float restLength) :
	m_other(other),
	m_springConstant(springConstant),
	m_restLength(restLength)
{
}

ParticleSpring::~ParticleSpring()
{
	if(m_other)
	{
		delete m_other;
	}
}

void ParticleSpring::updateForce(Particle* particle, float delta)
{
	Vector3 force = particle->getPosition();
	force -= m_other->getPosition();

	float magnitude = force.length();
	magnitude = fabsf(magnitude - m_restLength);
	magnitude *= m_springConstant;

	force = force.normalised();
	force *= -magnitude;
	particle->addForce(force);
}