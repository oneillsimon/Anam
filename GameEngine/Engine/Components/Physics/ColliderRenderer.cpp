#include "ColliderRenderer.h"

ColliderRenderer::ColliderRenderer(PhysicsComponent* component, const Colour& colour) :
	MeshRenderer(Mesh(getMeshFromCollider(component->getCollider()->getType())), Material("", TEXTURE_BLANK, colour)),
	m_colour(colour),
	m_component(component)
{
}

void ColliderRenderer::render(const Shader& shader, const  RenderingEngine& renderingEngine, const Camera& camera) const
{
	Transform t = getTransform();

	if(m_component->getCollider()->getType() == Collider::BOX)
	{
		ColliderBox& b = *(ColliderBox*)m_component->getCollider();

		t.setScale(b.getHalfSize());
	}
	else if(m_component->getCollider()->getType() == Collider::SPHERE)
	{
		ColliderSphere& s = *(ColliderSphere*)m_component->getCollider();

		t.setScale(s.getRadius());
	}

	else if(m_component->getCollider()->getType() == Collider::PLANE)
	{
		ColliderPlane& p = *(ColliderPlane*)m_component->getCollider();

		t.setScale((p.getNormal() - 1).absolute() * 20);
	}

	m_mesh.getWireFrameShader().bind();
	m_mesh.getWireFrameShader().updateUniforms(t, renderingEngine, camera, m_material);
	m_mesh.getWireFrameShader().setUniform("wireFrameColour", m_colour);
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
	case Collider::PLANE:
		return "plane.obj";
		break;
	default:
		return "sphere.obj";
		break;
	}
}