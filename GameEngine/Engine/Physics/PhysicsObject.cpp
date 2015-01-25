#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(RigidBody* rigidBody, Collider* collider, const Vector3& position, const Quaternion& rotation, float scale) :
	GameObject(position, rotation, scale),
	m_collider(collider)
{
	m_collider->m_body = rigidBody;
}

void PhysicsObject::initialise()
{
	m_collider->m_owner = getTransform();
	m_collider->m_body->m_owner = getTransform();

	Matrix3 tensor;
	tensor.setBlockInteriaTensor(getTransform()->getScale(), m_collider->m_body->getMass());
	m_collider->m_body->setIntertiaTensor(tensor);

	m_collider->m_body->calculateDerivedData();
	GameObject::initialise();
}

void PhysicsObject::update(float delta)
{
	m_collider->m_body->integrate(delta);

	GameObject::update(delta);
}