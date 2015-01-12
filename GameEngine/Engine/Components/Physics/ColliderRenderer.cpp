#include "ColliderRenderer.h"

ColliderRenderer::ColliderRenderer(Collider* collider, const Colour& defaultColour, const Colour& collidingColour) :
MeshRenderer(Mesh(getMeshFromCollider(collider->getType())), Material("", TEXTURE_BLANK, defaultColour)),
m_defaultColour(defaultColour),
m_collidingColour(collidingColour),
m_collider(collider)
{
}

void ColliderRenderer::render(const Shader& shader, const  RenderingEngine& renderingEngine, const Camera& camera) const
{
	Colour c = m_defaultColour;

	if(m_collider->getIsColliding())
	{
		c = m_collidingColour;
	}

	m_mesh.getWireFrameShader().bind();
	m_mesh.getWireFrameShader().updateUniforms(getTransform(), renderingEngine, camera, m_material);
	m_mesh.getWireFrameShader().setUniform("wireFrameColour", c);
	m_mesh.drawWireFrame();
}

std::string ColliderRenderer::getMeshFromCollider(int type)
{
	switch(type)
	{
	case Collider::TYPE_SPHERE:
		return "sphere.obj";
		break;
	case Collider::TYPE_AABB:
		return "cube.obj";
		break;
	}
}