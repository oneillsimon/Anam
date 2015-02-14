#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(const Vector3& position, const Quaternion& rotation, float scale) :
	GameObject(position, rotation, scale)
{
	//m_collider = new ColliderSphere(1);
	//float s = random(01, 5);
	m_collider = new ColliderBox(Vector3(1, 1, 1));
	m_collider->m_body = new RigidBody(10, 0.1f, 0.1f);
	m_collider->m_body->m_parent = this;
}

void PhysicsObject::initialise()
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			
		}
	}
	Matrix3 tensor;
	float mass = 8.0f;
	tensor.setBlockInteriaTensor(Vector3(1, 1, 1), mass);
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