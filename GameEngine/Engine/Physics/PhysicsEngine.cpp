#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
	mainTree = new Octree(Vector3(-100, -100, -100), Vector3(100, 100, 100), 1);
}

void PhysicsEngine::addObject(PhysicsObject* object)
{
	m_objects.push_back(object);
	mainTree->add(object);
}

void PhysicsEngine::simulate(float delta)
{
	//advance(m_objects, mainTree, delta);
	mainTree->advanceState(m_objects, delta);
}

void PhysicsEngine::handleCollisions()
{
}

PhysicsObject* PhysicsEngine::getObject(unsigned int index) const
{
	return m_objects[index];
}