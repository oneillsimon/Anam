#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(RigidBody* rigidBody, const Vector3& position, const Quaternion& rotation, float scale) :
	GameObject(position, rotation, scale)
{
	collider = new CollisionSphere();
	collider->m_radius = 1.0f;
	collider->m_body = rigidBody;
	addComponent(collider->m_body);
	addComponent(collider);
}

void PhysicsObject::update(float delta)
{
	collider->m_body->integrate(delta);

	GameObject::update(delta);
}