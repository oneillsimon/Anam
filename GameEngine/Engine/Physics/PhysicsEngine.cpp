#include <thread>

#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() :
resolver(256)
{
	int size = 20;
	m_tree = new Octree(Vector3(-size, -size, -size), Vector3(size, size, size), 1);

	cData.m_contactArray = contacts;
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
	m_tree->potentialCollisions(&cData);
	resolver.resolveContacts(cData.m_contactArray, cData.m_contactCount, delta);
	//cData.reset(256);
	ProfileTimers::physicsTimer.stopInvocation();
}

void PhysicsEngine::updateObjectReferences(std::vector<PhysicsObject*>& objects, Octree* octree, float delta)
{
	for(int i = 0; i < objects.size(); i++)
	{
		if(objects[i]->getTransform()->hasChanged())
		{
			octree->objectMoved(objects[i], objects[i]->getTransform()->getPosition());
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