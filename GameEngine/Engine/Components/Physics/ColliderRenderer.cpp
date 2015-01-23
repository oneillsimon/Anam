#include "ColliderRenderer.h"

ColliderRenderer::ColliderRenderer(Collider* collider, const Colour& defaultColour, const Colour& collidingColour) :
MeshRenderer(Mesh(getMeshFromCollider(collider->m_type)), Material("", TEXTURE_BLANK, defaultColour)),
m_defaultColour(defaultColour),
m_collidingColour(collidingColour),
m_collider(collider)
{
}

ColliderRenderer::ColliderRenderer(ColliderPlane* collider, const Colour& defaultColour, const Colour& collidingColour) :
MeshRenderer(Mesh("plane.obj"), Material("", TEXTURE_BLANK, defaultColour)),
m_defaultColour(defaultColour),
m_collidingColour(collidingColour),
m_collider(0)
{
}

void ColliderRenderer::render(const Shader& shader, const  RenderingEngine& renderingEngine, const Camera& camera) const
{
	Colour c = m_defaultColour;

	m_mesh.getWireFrameShader().bind();
	m_mesh.getWireFrameShader().updateUniforms(getTransform(), renderingEngine, camera, m_material);
	m_mesh.getWireFrameShader().setUniform("wireFrameColour", c);
	m_mesh.drawWireFrame();
}

std::string ColliderRenderer::getMeshFromCollider(int type)
{
	switch(type)
	{
	case Collider::SPHERE:
		return "sphere.obj";
		break;
	case Collider::BOX:
		return "cube.obj";
		break;
	}
}