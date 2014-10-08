#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "../Core/GameComponent.h"
#include "Particle.h"
#include "PFGen.h"

class TestComp : public GameComponent
{
private:
	ParticleForceRegistry* m_registry;

public:
	TestComp(ParticleForceRegistry* registry);
	~TestComp();

	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;
};

#endif