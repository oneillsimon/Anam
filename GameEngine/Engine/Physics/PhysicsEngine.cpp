#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() :
	resolver(MAX_CONTACT_COUNT * 8)
{
	cData.reset(MAX_CONTACT_COUNT * 8);
	cData.m_contactArray = contacts;
	int size = 120;
	m_tree = new Octree(0, Vector3(), Vector3(size, size, size));
}

void PhysicsEngine::addObject(PhysicsObject* object)
{
	m_objects.push_back(object);
	m_tree->addObject(object);
}

void PhysicsEngine::simulate(float delta)
{
	if(m_objects.empty())
	{
		return;
	}

	ProfileTimers::physicsTimer.startInvocation();
	m_tree->potentialCollisions(&cData);
	handleCollisions(delta);
	
	cData.reset(MAX_CONTACT_COUNT * 8);
	ProfileTimers::physicsTimer.stopInvocation();
}

void PhysicsEngine::handleCollisions(float delta)
{
	//printf("contact count %d\n", cData.m_contactCount);

	if(cData.m_contactCount > 0)
	{
		int uu = 0;
	}

	//resolver.resolveContacts(cData.m_contactArray, cData.m_contactCount, delta);
}

PhysicsObject* PhysicsEngine::getObject(unsigned int index) const
{
	return m_objects[index];
}