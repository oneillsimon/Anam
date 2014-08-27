#include "Collider.h"
#include "BoundingSphere.h"

Collider::Collider(int type, const std::string& meshName) :
	ReferenceCounter(),
	m_type(type),
	m_mesh(meshName)
{
}

IntersectionData Collider::intersect(const Collider& collider) const
{
	if(m_type == TYPE_SPHERE && collider.getType() == TYPE_SPHERE)
	{
		BoundingSphere* self = (BoundingSphere*)this;
		return self->intersectBoundingSphere((BoundingSphere&)collider);
	}
}

int Collider::getType() const
{
	return m_type;
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