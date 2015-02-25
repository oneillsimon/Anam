#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(const Vector3& position, const Quaternion& rotation, float scale) :
	GameObject(position, rotation, scale)
{
	//m_collider = new ColliderSphere(hE[0]);
	//float s = random(01, 5);
	
}

void PhysicsObject::initialise()
{

	GameObject::initialise();
}

void PhysicsObject::update(float delta)
{
	m_collider->getBody()->integrate(delta);
	GameObject::update(delta);
}

Collider* PhysicsObject::getCollider()
{
	return m_collider;
}

void PhysicsObject::setCollider(Collider* collider, RigidBody* body)
{
	m_collider = collider;
	body->setParent(this);
	m_collider->setBody(body);
}