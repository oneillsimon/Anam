#include "Collider.h"

Collider::Collider(int type) :
m_type(type)
{
	m_isColliding = false;
}

IntersectionData Collider::intersect(const Collider& collider) const
{
	if(m_type == TYPE_SPHERE && collider.getType() == TYPE_SPHERE)
	{
		BoundingSphere* self = (BoundingSphere*)this;
		IntersectionData intersectData = self->intersectBoundingSphere((BoundingSphere&)collider);
		m_isColliding = intersectData.getDoesIntersect();
		collider.m_isColliding = m_isColliding;
		return intersectData;
	}

	if(m_type == TYPE_AABB && collider.getType() == TYPE_AABB)
	{
		AABB* self = (AABB*)this;
		IntersectionData intersectData = self->intersectAABB((AABB&)collider);
		m_isColliding = intersectData.getDoesIntersect();
		collider.m_isColliding = m_isColliding;
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

Vector3 Collider::getScale() const
{
	return m_scale;
}