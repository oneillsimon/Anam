#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(PhysicsObject* obj) :
	m_obj(obj)
{
}

void RigidBodyComponent::input(const Input& input, float delta)
{
	if(input.getKey(Input::KEY_1))
	{
		m_obj->addForce(m_obj->getTransform()->getPosition(), Vector3(0, -9.8f * 5, 0) * m_obj->getMass());
	}
}

void RigidBodyComponent::update(float delta)
{
}