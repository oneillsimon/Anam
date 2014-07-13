#ifndef PARTICLE_H
#define PARTICLE_H

#include "../Core/Math3D.h"
#include "../Core/GameObject.h"

class Particle : public GameObject
{
private:
	Vector3 velocity;
	Vector3 acceleration;
	
	float damping;
	float inverseMass;
	Vector3 forceAccumulated;

public:
	Particle(Vector3 velocity, Vector3 acceleration, float mass, float damping);
	~Particle();

	virtual void update(float delta);

	void addForce(const Vector3& v);
	void clearAccumulator();
	bool hasFiniteMass();

	Vector3 getVelocity() const;
	Vector3 getAcceleration() const;
	
	float getDamping() const;
	float getMass() const;
	float getInverseMass() const;
	Vector3 getForceAccumulated() const;

	void setVelocity(const Vector3& v);
	void setAcceleration(const Vector3& v);

	void setDamping(float f);
	void setMass(float mass);
};

#endif