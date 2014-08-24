#include "PhysicsObjectComponent.h"

PhysicsObjectComponent::PhysicsObjectComponent(const PhysicsObject* object) :
	m_physicsObject(object)
{
}

void PhysicsObjectComponent::update(float delta)
{
	getTransform()->setPosition(m_physicsObject->getPosition());
}