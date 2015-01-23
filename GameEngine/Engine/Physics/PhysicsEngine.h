#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#define MAX_CONTACT_COUNT 256

#include <vector>

#include "../Core/Profiling.h"
#include "Contacts.h"
#include "Octree.h"
#include "PhysicsObject.h"

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
	void handleCollisions(float delta);

	PhysicsObject* getObject(unsigned int index) const;
};

#endif