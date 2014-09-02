#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "../Components/GameComponent.h"
#include "Particle.h"

class TestComp : public GameComponent
{
private:
	Particle particle;

public:
	TestComp(const Particle& particle);
	virtual void update(float delta);
};

#endif