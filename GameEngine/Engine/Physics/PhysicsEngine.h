#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <vector>

#include "PhysicsObject.h"

class PhysicsEngine
{
private:
	std::vector<PhysicsObject*> m_objects;

public:
	PhysicsEngine();

	void addObject(PhysicsObject* object);
	void simulate(const PhysicsObject& object, float delta);
	void handleCollisions();

	PhysicsObject* getObject(unsigned int index) const;
};

#endif