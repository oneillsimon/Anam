#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
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
	mainTree->potentialCollisions(collisions);
	//printf("col count: %d\n", collisions.size());
	collisions.clear();
}

void PhysicsEngine::handleCollisions()
{
}

PhysicsObject* PhysicsEngine::getObject(unsigned int index) const
{
	return m_objects[index];
}