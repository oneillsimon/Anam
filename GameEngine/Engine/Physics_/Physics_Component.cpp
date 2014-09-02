#include "Physics_Component.h"

TestComp::TestComp(const Particle& particle) :
	particle(particle)
{
}

void TestComp::update(float delta)
{
	//getTransform()->setPosition(particle.getPosition());
	particle.integrate(delta);
}

void TestComp::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
	glDisable(GL_CULL_FACE);
	Transform t = Transform(particle.getPosition());
	shader.bind();
	shader.updateUniforms(t, renderingEngine, camera, particle.getShape().getMaterial());
	particle.getShape().draw();
	glEnable(GL_CULL_FACE);
}