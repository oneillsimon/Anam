#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <vector>

#include "../Core/Profiling.h"
#include "Octree.h"
#include "PhysicsComponent.h"
#include "Collision.h"

class PhysicsEngine
{
private:
	std::vector<PhysicsComponent*> m_components;

	CollisionData cData;
	ContactResolver resolver;
	Contact contacts[MAX_CONTACTS];

public:
	Octree* m_tree;
	PhysicsEngine();

	void addComponent(PhysicsComponent* object);
	void simulate(float delta);
	void updateComponentReferences(std::vector<PhysicsComponent*>& components, Octree* octree, float delta);

	//PhysicsObject* getObject(unsigned int index) const;
	//std::vector<PhysicsObject*>* getObjects();
};

#endif