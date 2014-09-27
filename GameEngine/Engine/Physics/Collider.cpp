#include "Collider.h"

Collider::Collider(int type, const std::string& meshName) :
	ReferenceCounter(),
	m_type(type),
	m_mesh(meshName)
{
	m_isColliding = false;
}

void Collider::update(float delta)
{

}

void Collider::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
	m_mesh.getWireFrameShader().bind();
	m_mesh.getWireFrameShader().updateUniforms(*m_parent->getTransform(), renderingEngine, camera);

	if(m_isColliding)
	{
		m_mesh.getWireFrameShader().setUniform("wireFrameColour", COLOUR_YELLOW.toGLSLVec4());
	}
	else
	{
		m_mesh.getWireFrameShader().setUniform("wireFrameColour", COLOUR_WHITE.toGLSLVec4());
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
	return Vector3(0, 0, 0);
}

Mesh Collider::getMesh() const
{
	return m_mesh;
}

Vector3 Collider::getScale() const
{
	return m_scale;
}