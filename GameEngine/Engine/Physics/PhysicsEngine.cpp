#include <thread>

#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
	int size = 20;
	//m_tree = new Octree(0, 0, Vector3(), Vector3(size, size, size));
	m_altTree = new OctreeAlt(Vector3(-size, -size, -size), Vector3(size, size, size), 1);
}

void PhysicsEngine::addObject(PhysicsObject* object)
{
	m_objects.push_back(object);
	m_altTree->add(object);
	//m_tree->addObject(object);
	//m_tree->m_allObjects.push_back(object);
}

void PhysicsEngine::simulate(float delta)
{
	if(m_objects.empty())
	{
		return;
	}

	ProfileTimers::physicsTimer.startInvocation();

	//std::thread t1(&PhysicsEngine::updateObjectReferences, this, m_objects, m_altTree, delta);
	//t1.join();
	updateObjectReferences(m_objects, m_altTree, delta);
	m_altTree->potentialCollisions();
	//m_tree->potentialCollisions();
	//m_tree->recalculate();
	ProfileTimers::physicsTimer.stopInvocation();
}

void PhysicsEngine::updateObjectReferences(std::vector<PhysicsObject*>& objects, OctreeAlt* octree, float delta)
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