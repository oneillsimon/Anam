#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(const RigidBody& rigidBody, const Collider& collider) :
	m_rigidBody(rigidBody),
	m_collider(collider)
{
}

void PhysicsComponent::update(float delta)
{
}

void PhysicsComponent::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera)
{
	m_collider.render(shader, renderingEngine, camera);
}