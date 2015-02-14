#include "../../Physics/PhysicsObject.h"
#include "ColliderRenderer.h"

ColliderRenderer::ColliderRenderer(bool isPhysX, Collider* collider, const Colour& defaultColour, const Colour& collidingColour) :
MeshRenderer(Mesh(getMeshFromCollider(collider->m_type)), Material("", TEXTURE_BLANK, defaultColour)),
m_defaultColour(defaultColour),
m_collidingColour(collidingColour),
m_collider(collider),
m_isPhysX(isPhysX)
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

	Transform t = getTransform();

	if(m_collider->m_type == Collider::BOX)
	{
		ColliderBox& b = *(ColliderBox*)m_collider;

		t.setScale(b.m_halfSize);
		t.setRotation(b.m_parent->getTransform()->getRotation());
	}
	else if(m_collider->m_type == Collider::SPHERE)
	{
		ColliderSphere& s = *(ColliderSphere*)m_collider;

		t.setScale(s.m_radius);
		t.setRotation(Quaternion());
	}

	m_mesh.getWireFrameShader().bind();
	m_mesh.getWireFrameShader().updateUniforms(t, renderingEngine, camera, m_material);
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
	case Collider::OBB:
		return "cube.obj";
		break;
	case Collider::BOX:
		return "cube.obj";
		break;
	case Collider::PLANE:
		return "plane.obj";
		break;
	}
}