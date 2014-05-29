#include "MeshRenderer.h"
#include "../Rendering/Shader.h"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
{
	m_mesh = mesh;
	m_material = material;
}

MeshRenderer::~MeshRenderer()
{
	if(m_mesh)
	{
		delete m_mesh;
	}

	if(m_material)
	{
		delete m_material;
	}
}

void MeshRenderer::render(Shader* shader, RenderingEngine* renderingEngine)
{
	shader->bind();
	Transform t = getTransform();
	shader->updateUniforms(t, *m_material, renderingEngine);
	m_mesh->draw();
}