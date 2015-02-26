#include "../Core/CoreEngine.h"
#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(Collider* collider, RigidBody* rigidBody) :
	m_collider(collider)
{
	m_collider->setBody(rigidBody);
}

void PhysicsComponent::initialise()
{
	if(m_collider->getBody())
	{
		m_collider->getBody()->setParent(this);

		m_collider->getBody()->setOrientation(m_parent->getTransform()->getRotation());
		m_collider->getBody()->setVelocity(Vector3(0, 0, 0));
		m_collider->getBody()->setRotation(Vector3(0, 0, 0));

		Vector3 extents = m_collider->getExtents();
		float mass = extents[0] * extents[1] * extents[2] * 8.0f;
		m_collider->getBody()->setMass(mass);

		Matrix3 tensor;
		tensor.setBlockInteriaTensor(extents, mass);
		m_collider->getBody()->setIntertiaTensor(tensor);

		m_collider->getBody()->clearAccumulators();

		m_collider->getBody()->setCanSleep(true);
		m_collider->getBody()->setAwake();

		m_collider->getBody()->calculateDerivedData();
	}
	
	m_parent->getEngine()->getPhysicsEngine()->addComponent(this);
}

void PhysicsComponent::update(float delta)
{
	if(m_collider->getBody())
	{
		m_collider->getBody()->integrate(delta);
	}
	GameComponent::update(delta);
}

Collider* PhysicsComponent::getCollider()
{
	return m_collider;
}