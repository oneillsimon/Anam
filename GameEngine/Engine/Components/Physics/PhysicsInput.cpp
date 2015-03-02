#include "PhysicsInput.h"

PhysicsInput::PhysicsInput(const PhysicsComponent& componet, int key1, int key2, int key3, int key4) :
	m_component(componet),
	m_key1(key1),
	m_key2(key2),
	m_key3(key3),
	m_key4(key4)
{
}

void PhysicsInput::input(const Input& input, float delta)
{
	if(input.getKey(m_key1))
	{
		m_component.getBody()->addForce(Vector3(0, -9.8f, 0));
	}
}

void PhysicsInput::update(float delta)
{
	m_component.getBody()->addForce(Vector3(0, -9.8f * m_component.getBody()->getMass(), 0));
}