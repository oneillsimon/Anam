#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "../Physics_/RigidBody.h"
#include "Collider.h"

class PhysicsObject : public GameObject
{
private:
	Vector3 m_velocity;
	Collider* m_collider;
	RigidBody* m_rigidBody;

public:
	PhysicsObject(const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(), float scale = 1.0f, Vector3 velocity = Vector3());
	virtual ~PhysicsObject();
	
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;

	void integrate(float delta);

	Vector3 getVelocity() const;
	Collider* getCollider();
	RigidBody* getRigidBody();

	void setVelocity(const Vector3& velocity);
	void setCollider(Collider* collider);
	void setRigidBody(RigidBody* rigidBody);

	PhysicsObject operator =(PhysicsObject other);
};

#endif