#ifndef PARTICLE_H
#define PARTICLE_H

#include "../Core/Math3D.h"
#include "../Rendering/Shape.h"

class Particle
{
private:
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_acceleration;
	float m_damping;
	float m_inverseMass;
	Vector3 m_forceAccumulated;
	Shape m_shape;
	

public:
	Particle(const Shape& shape);
	Particle(const Particle& other);

	void integrate(float delta);
	void addForce(const Vector3& force);
	void resetAcummulator() const;

	Vector3 getPosition() const;
	Vector3 getVelocity() const;
	Vector3 getAcceleration() const;
	float getDamping() const;
	float getInverseMass() const;
	float getMass();
	Vector3 getAccumulatedForce() const;
	Shape getShape() const;

	void setPosition(const Vector3& position);
	void setVelocity(const Vector3& velocity);
	void setAcceleration(const Vector3& acceleration);
	void setDamping(float damping);
	void setMass(float mass);

	Particle operator =(const Particle& other);
};

#endif