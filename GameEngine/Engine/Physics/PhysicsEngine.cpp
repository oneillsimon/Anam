#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
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
	m_tree->potentialCollisions();
	ProfileTimers::physicsTimer.stopInvocation();
}

PhysicsObject* PhysicsEngine::getObject(unsigned int index) const
{
	return m_objects[index];
}