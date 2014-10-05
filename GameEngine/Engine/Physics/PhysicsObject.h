#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "Collider.h"

class PhysicsObject : public GameObject
{
private:
	Vector3 m_velocity;
	Collider* m_collider;

public:
	PhysicsObject(const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(), float scale = 1.0f, Vector3 velocity = Vector3());
	virtual ~PhysicsObject();
	
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;

	void integrate(float delta);

	const Vector3& getVelocity() const;
	const Collider& getCollider() const;

	void setVelocity(const Vector3& velocity);
	void setCollider(Collider* collider);

	PhysicsObject operator =(PhysicsObject other);
};

#endif