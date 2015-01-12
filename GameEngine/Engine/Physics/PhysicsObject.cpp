#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(RigidBody* rigidBody, Collider* collider, const Vector3& position, const Quaternion& rotation, float scale) :
	m_rigidBody(rigidBody),
	m_collider(collider),
	GameObject(position, rotation, scale)
{
	addComponent(m_rigidBody);
	addComponent(m_collider);
}

void PhysicsObject::update(float delta)
{
	m_rigidBody->integrate(delta);

	GameObject::update(delta);
}