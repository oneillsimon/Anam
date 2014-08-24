#include "Collider.h"
#include "BoundingSphere.h"

Collider::Collider(int type) :
	ReferenceCounter(),
	m_type(type)
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