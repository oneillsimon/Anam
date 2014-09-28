#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <vector>

#include "PhysicsObject.h"

class PhysicsEngine
{
private:
	std::vector<PhysicsObject> m_objects;

public:
	PhysicsEngine();

	void addObject(const PhysicsObject& object);
	void simulate(const GameObject& object, float delta);
	void handleCollisions();

	//TODO: temp
	const PhysicsObject& getObject(unsigned int index) const
	{
		return m_objects[index];
	}

	unsigned int getNumObjects() const
	{
		return (unsigned int)m_objects.size();
	}
};

#endif