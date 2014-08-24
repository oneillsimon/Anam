#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "../Core/Math3D.h"
#include "BoundingSphere.h"

class PhysicsObject
{
private:
	Vector3 m_position;
	Vector3 m_velocity;

	Vector3 m_oldPosition;

	//TEMP
	Collider* m_collider;

public:
	PhysicsObject(Collider* collider, const Vector3& velocity);
	PhysicsObject(const PhysicsObject& other);
	virtual ~PhysicsObject();

	void integrate(float delta);

	const Vector3& getPosition() const;
	const Vector3& getVelocity() const;
	const Collider& getCollider();

	void setVelocity(const Vector3& velocity);

	PhysicsObject operator =(PhysicsObject other);
};

#endif