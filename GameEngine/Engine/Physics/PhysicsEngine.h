#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <vector>

#include "Octree\Octree.h"
#include "PhysicsObject.h"

class PhysicsEngine
{
private:
	std::vector<PhysicsObject*> m_objects;
	std::vector<IntersectionData> collisions;

public:
	Octree* mainTree;
	PhysicsEngine();

	void addObject(PhysicsObject* object);
	void simulate(float delta);
	void handleCollisions();

	PhysicsObject* getObject(unsigned int index) const;
};

#endif