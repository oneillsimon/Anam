#include "MeshRenderer.h"

void MeshRenderer::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
	shader.bind();
	shader.updateUniforms(getTransform(), renderingEngine, camera, m_material);
	m_mesh.draw();

#if MESH_DRAW_WIREFRAMES == 0
	m_mesh.getWireFrameShader().bind();
	m_mesh.getWireFrameShader().updateUniforms(getTransform(), renderingEngine, camera, m_material);
	m_mesh.getWireFrameShader().setUniform("wireFrameColour", COLOUR_RED.toGLSLVec4());
	m_mesh.drawWireFrame();
#endif
}