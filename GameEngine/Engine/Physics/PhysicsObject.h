#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "Collider.h"

class PhysicsObject : public GameObject
{
private:
	Vector3 m_velocity;

	//Vector3 m_oldPosition;

	//TEMP
	Collider* m_collider;

public:
	//PhysicsObject(Collider* collider, const Vector3& velocity);
	//PhysicsObject(const PhysicsObject& other);
	PhysicsObject(const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(), float scale = 1.0f);
	virtual ~PhysicsObject();

	void integrate(float delta);

	//const Vector3& getPosition() const;
	const Vector3& getVelocity() const;
	const Collider& getCollider() const;

	void setVelocity(const Vector3& velocity);

	PhysicsObject operator =(PhysicsObject other);
};

#endif