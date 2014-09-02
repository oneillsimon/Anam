#include "Physics_Component.h"

TestComp::TestComp(const Particle& particle)
{
	this->particle = particle;
}

void TestComp::update(float delta)
{
	getTransform()->setPosition(particle.getPosition());
	particle.integrate(delta);
}