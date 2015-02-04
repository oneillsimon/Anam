#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <vector>

#include "../Core/Profiling.h"
#include "Octree.h"
#include "PhysicsObject.h"

class PhysicsEngine
{
private:
	std::vector<PhysicsObject*> m_objects;

public:
	OctreeAlt* m_altTree;
	Octree* m_tree;
	PhysicsEngine();

	void addObject(PhysicsObject* object);
	void simulate(float delta);
	void updateObjectReferences(std::vector<PhysicsObject*>& objects, OctreeAlt* octree, float delta);

	PhysicsObject* getObject(unsigned int index) const;
};

#endif