#include "MeshRenderer.h"

void MeshRenderer::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
	shader.bind();
	shader.updateUniforms(getTransform(), m_material, renderingEngine, camera);
	m_mesh.draw();

#if MESH_DRAW_WIREFRAMES == 0
	m_mesh.getMeshData().getWireFrameShader().bind();
	m_mesh.getMeshData().getWireFrameShader().updateUniforms(getTransform(), m_material, renderingEngine, camera);
	m_mesh.getMeshData().getWireFrameShader().setUniform("wireFrameColour", COLOUR_RED.toGLSLVec4());
	m_mesh.drawWireFrame();
#endif
}