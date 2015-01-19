#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() :
	resolver(256*8)
{
	cData.m_contactArray = contacts;
	int size = 12;
	mainTree = new Octree(Vector3(-size, -size, -size), Vector3(size, size, size), 1);
}

void PhysicsEngine::addObject(PhysicsObject* object)
{
	m_objects.push_back(object);
	mainTree->add(object);
}

void PhysicsEngine::simulate(float delta)
{
	if(m_objects.empty())
	{
		return;
	}

	mainTree->refreshObjects(m_objects);
	mainTree->potentialCollisions(&cData);
	handleCollisions(delta);
}

void PhysicsEngine::handleCollisions(float delta)
{
	printf("contact count %d\n", cData.m_contactCount);
	resolver.resolveContacts(cData.m_contactArray, cData.m_contactCount, delta);
}

PhysicsObject* PhysicsEngine::getObject(unsigned int index) const
{
	return m_objects[index];
}