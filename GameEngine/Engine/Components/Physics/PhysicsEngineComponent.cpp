#include "PhysicsEngineComponent.h"

PhysicsEngineComponent::PhysicsEngineComponent(const PhysicsEngine& engine) :
	m_physicsEngine(engine)
{
}

void PhysicsEngineComponent::update(float delta)
{
	//m_physicsEngine.simulate(delta);
	m_physicsEngine.handleCollisions();
}

const PhysicsEngine& PhysicsEngineComponent::getPhysicsEngine()
{
	return m_physicsEngine;
}