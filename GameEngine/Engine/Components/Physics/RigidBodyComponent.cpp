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
		m_obj->getCollider()->m_body->addForce(Vector3(0, -9.8f * 5, 0));
	}

	if(input.getKey(Input::KEY_2))
	{
		m_obj->getCollider()->m_body->setAwake();
		m_obj->getCollider()->m_body->addRotation(Vector3(0, 5, 0));
	}
}

void RigidBodyComponent::update(float delta)
{
}