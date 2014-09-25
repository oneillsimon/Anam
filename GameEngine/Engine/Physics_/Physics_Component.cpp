#include "Physics_Component.h"
#include "PFGen.h"

TestComp::TestComp(ParticleForceRegistry* registry)
{
	m_registry = new ParticleForceRegistry(*registry);
}

TestComp::~TestComp()
{
	if(m_registry)
	{
		delete m_registry;
	}
}

void TestComp::input(const Input& input, float delta)
{
}

void TestComp::update(float delta)
{
	m_registry->updateForces(delta);
}

void TestComp::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
	for(unsigned int i = 0; i < m_registry->getLength(); i++)
	{
		Particle p = *m_registry->getParticle(i);
		
		glDisable(GL_CULL_FACE);
		Transform t = Transform(p.getPosition());
		shader.bind();
		shader.updateUniforms(t, renderingEngine, camera, p.getShape().getMaterial());
		p.getShape().draw();
		glEnable(GL_CULL_FACE);
	}
}