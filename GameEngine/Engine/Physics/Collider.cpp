#include "Collider.h"

Collider::Collider(int type, const std::string& meshName) :
ReferenceCounter(),
m_type(type),
m_mesh(meshName)
{
	m_isColliding = false;
}

void Collider::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
	m_mesh.getWireFrameShader().bind();
	Transform t = *m_parent->getTransform();
	t.setScale(m_scale);
	m_mesh.getWireFrameShader().updateUniforms(t, renderingEngine, camera);

	if(m_isColliding)
	{
		m_mesh.getWireFrameShader().setUniform("wireFrameColour", COLOUR_RED.toGLSLVec4());
	}
	else
	{
		m_mesh.getWireFrameShader().setUniform("wireFrameColour", COLOUR_LIME.toGLSLVec4());
	}

	m_mesh.drawWireFrame();
}

IntersectionData Collider::intersect(const Collider& collider) const
{
	if(m_type == TYPE_SPHERE && collider.getType() == TYPE_SPHERE)
	{
		BoundingSphere* self = (BoundingSphere*)this;
		IntersectionData intersectData = self->intersectBoundingSphere((BoundingSphere&)collider);
		m_isColliding = intersectData.getDoesIntersect();
		return intersectData;
	}

	if(m_type == TYPE_AABB && collider.getType() == TYPE_AABB)
	{
		AABB* self = (AABB*)this;
		IntersectionData intersectData = self->intersectAABB((AABB&)collider);
		m_isColliding = intersectData.getDoesIntersect();
		return intersectData;
	}
}

int Collider::getType() const
{
	return m_type;
}

bool Collider::getIsColliding() const
{
	return m_isColliding;
}

Vector3 Collider::getCentre() const
{
	return m_parent->getTransform()->getPosition();
}

Mesh Collider::getMesh() const
{
	return m_mesh;
}

Vector3 Collider::getScale() const
{
	return m_scale;
}