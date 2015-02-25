#include <iostream>

#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(PhysicsObject* obj) :
	m_obj(obj)
{
}

void RigidBodyComponent::input(const Input& input, float delta)
{
	if(input.getKey(Input::KEY_1))
	{
		m_obj->getCollider()->m_body->setAwake();
		//m_obj->getCollider()->m_body->setAcceleration(Vector3(0, 1.0f, 0));
		m_obj->getCollider()->m_body->addForce(Vector3(0, -9.8f * m_obj->getCollider()->m_body->getMass(), 0));
	}

	if(input.getKey(Input::KEY_2))
	{
		m_obj->getCollider()->m_body->setAwake();
		//m_obj->getCollider()->m_body->setAcceleration(Vector3(0, 1.0f, 0));
		m_obj->getCollider()->m_body->addForce(Vector3(0, 9.8f * m_obj->getCollider()->m_body->getMass(), 0));
	}

	if(input.getKey(Input::KEY_3))
	{
		m_obj->getCollider()->m_body->setAwake();
		m_obj->getCollider()->m_body->addTorque(AXIS_X * toRadians(100));
	}
}

void RigidBodyComponent::update(float delta)
{
	//m_obj->getTransform()->rotate(Quaternion(AXIS_Z, toRadians(1)));
	//m_obj->m_collider->m_body->addForce(Vector3(0, -12.8f, 0));
}