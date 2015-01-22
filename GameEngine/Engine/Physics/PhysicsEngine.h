#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#define MAX_CONTACT_COUNT 256

#include <vector>

#include "Octree/Octree.h"
#include "PhysicsObject.h"
#include "../Physics_/Contacts.h"
#include "IntersectionData.h"

class PhysicsEngine
{
private:
	std::vector<PhysicsObject*> m_objects;
	std::vector<IntersectionData> collisions;
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