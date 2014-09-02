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
	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;
};

#endif