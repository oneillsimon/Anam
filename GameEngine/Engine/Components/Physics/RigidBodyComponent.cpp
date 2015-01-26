#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(RigidBody* body) :
	m_body(body)
{
}

void RigidBodyComponent::input(const Input& input, float delta)
{
	if(input.getKey(Input::KEY_1))
	{
		m_body->setAwake();
		m_body->addForce(Vector3(0, -9.8f, 0));
	}
}

void RigidBodyComponent::update(float delta)
{
	//m_body->addForce(Vector3(0, -9.8f, 0));
}