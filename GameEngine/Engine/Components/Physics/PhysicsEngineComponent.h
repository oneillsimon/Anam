#ifndef PHYSICSENGINECOMPONENT_H
#define PHYSICSENGINECOMPONENT_H

#include "../../Core/GameComponent.h"
#include "../../Core/GameObject.h"
#include "../../Physics/PhysicsEngine.h"

class PhysicsEngineComponent : public GameComponent
{
private:
	PhysicsEngine m_physicsEngine;

public:
	PhysicsEngineComponent(const PhysicsEngine& engine);

	virtual void update(float delta);

	const PhysicsEngine& getPhysicsEngine();
};

#endif