#include <thread>

#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() :
resolver(MAX_CONTACTS)
{
	int size = 20;
	m_tree = new Octree(Vector3(-size, -size, -size), Vector3(size, size, size), 1);

	cData.setContactArray(contacts);
}

void PhysicsEngine::addObject(PhysicsObject* object)
{
	m_objects.push_back(object);
	m_tree->add(object);
}

void PhysicsEngine::simulate(float delta)
{
	if(m_objects.empty())
	{
		return;
	}

	ProfileTimers::physicsTimer.startInvocation();
	updateObjectReferences(m_objects, m_tree, delta);
	cData.reset(MAX_CONTACTS);
	m_tree->potentialCollisions(&cData);
	resolver.resolveContacts(cData.getContactArray(), cData.getContactCount(), delta);
	ProfileTimers::physicsTimer.stopInvocation();
}

void PhysicsEngine::updateObjectReferences(std::vector<PhysicsObject*>& objects, Octree* octree, float delta)
{
	for(int i = 0; i < objects.size(); i++)
	{
		if(objects[i]->getTransform()->hasChanged())
		{
			//octree->objectMoved(objects[i], objects[i]->getTransform()->getPosition());
		}
	}
}

PhysicsObject* PhysicsEngine::getObject(unsigned int index) const
{
	return m_objects[index];
}

std::vector<PhysicsObject*>* PhysicsEngine::getObjects()
{
	return &m_objects;
}