#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(const Vector3& position, const Quaternion& rotation, float scale) :
	GameObject(position, rotation, scale)
{
	//m_collider = new ColliderSphere(hE[0]);
	//float s = random(01, 5);
	
}

void PhysicsObject::initialise()
{
	Matrix3 tensor;
	//float mass = m_collider->m_body->getMass();
	float mass = m_collider->getExtents().scalarProduct(4.0f) + FLT_EPSILON;
	m_collider->m_body->setMass(mass);
	tensor.setBlockInteriaTensor(m_collider->getExtents(), mass);

	m_collider->m_body->setLinearDamping(0.95f);
	m_collider->m_body->setAngularDamping(0.8f);
	m_collider->m_body->clearAccumulators();

	m_collider->m_body->setCanSleep(true);
	m_collider->m_body->setAwake();
	m_collider->m_body->setIntertiaTensor(tensor);

	m_collider->m_body->calculateDerivedData();


	GameObject::initialise();
}

void PhysicsObject::update(float delta)
{
	m_collider->m_body->integrate(delta);
	GameObject::update(delta);
}

Collider* PhysicsObject::getCollider()
{
	return m_collider;
}

void PhysicsObject::setCollider(Collider* collider, RigidBody* body)
{
	m_collider = collider;
	m_collider->m_body = body;
	m_collider->m_body->m_parent = this;
}