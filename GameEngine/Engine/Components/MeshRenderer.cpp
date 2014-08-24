#include "MeshRenderer.h"

void MeshRenderer::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
	shader.bind();
	shader.updateUniforms(getTransform(), m_material, renderingEngine, camera);
	m_mesh.draw();
}