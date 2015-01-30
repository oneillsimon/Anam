#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#define MAX_CONTACT_COUNT 256

#include <vector>

#include "../Core/Profiling.h"
#include "Octree.h"
#include "PhysicsObject.h"

class PhysicsEngine
{
private:
	std::vector<PhysicsObject*> m_objects;

public:
	Octree* m_tree;
	PhysicsEngine();

	void addObject(PhysicsObject* object);
	void simulate(float delta);

	PhysicsObject* getObject(unsigned int index) const;
};

#endif