#include <thread>

#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() :
resolver(MAX_CONTACTS)
{
	float size = 12.0f;
	m_tree = new Octree(Vector3(-size, -size, -size), Vector3(size, size, size), 1);

	cData.setContactArray(contacts);
}

void PhysicsEngine::addComponent(PhysicsComponent* component)
{
	m_components.push_back(component);
	m_tree->add(component);
}

void PhysicsEngine::simulate(float delta)
{
	if(m_components.empty())
	{
		return;
	}

	cData.reset(MAX_CONTACTS);

	ProfileTimers::physicsTimer.startInvocation();
	ProfileTimers::physRefTimer.startInvocation();
	updateComponentReferences(m_components, m_tree, delta);
	ProfileTimers::physRefTimer.stopInvocation();
	m_tree->potentialCollisions(&cData);
	resolver.resolveContacts(cData.getContactArray(), cData.getContactCount(), delta);
	ProfileTimers::physicsTimer.stopInvocation();
}

void PhysicsEngine::updateComponentReferences(std::vector<PhysicsComponent*>& components, Octree* octree, float delta)
{
	for(unsigned i = 0; i < components.size(); i++)
	{
		if(components[i]->getTransform()->hasChanged())
		{
			octree->objectMoved(components[i], components[i]->getTransform()->getPosition());
		}
	}
}