#include "Collider.h"

ColliderSphere::ColliderSphere(float radius) :
m_radius(radius)
{
	m_type = Type::SPHERE;
}

Vector3 ColliderSphere::getExtents()
{
	return Vector3(m_radius, m_radius, m_radius);
}

ColliderBox::ColliderBox(const Vector3& halfExtents) :
m_halfSize(halfExtents)
{
	m_type = Type::BOX;
}

Vector3 ColliderBox::getExtents()
{
	return m_halfSize;
}

ColliderPlane::ColliderPlane(const Vector3& direction, float offset) :
m_direction(direction),
m_offset(offset)
{
	m_type = Type::PLANE;
}

void Collider::calculateInternals()
{
}