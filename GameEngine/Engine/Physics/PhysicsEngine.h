#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <vector>

#include "../Core/Profiling.h"
#include "Octree.h"
#include "PhysicsObject.h"
#include "Collision.h"

class PhysicsEngine
{
private:
	std::vector<PhysicsObject*> m_objects;
	
	CollisionData cData;
	ContactResolver resolver;
	Contact contacts[256];

public:
	Octree* m_tree;
	PhysicsEngine();

	void addObject(PhysicsObject* object);
	void simulate(float delta);
	void updateObjectReferences(std::vector<PhysicsObject*>& objects, Octree* octree, float delta);

	PhysicsObject* getObject(unsigned int index) const;
	std::vector<PhysicsObject*>* getObjects();
};

#endif