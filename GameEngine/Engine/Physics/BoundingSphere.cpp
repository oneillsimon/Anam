#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(Vector3 center, float radius) :
	Collider(Collider::TYPE_SPHERE),
	m_center(center),
	m_radius(radius)
{
}

IntersectionData BoundingSphere::intersectBoundingSphere(const BoundingSphere& other)
{
	float radiusDistance = m_radius + other.getRadius();
	Vector3 direction = (other.getCentre() - m_center);
	float centerDistance = direction.length();
	direction /= centerDistance;

	float distance = centerDistance - radiusDistance;

	return IntersectionData(distance < 0, direction * distance);
}

void BoundingSphere::transform(const Vector3& translation)
{
	m_center += translation;
}

Vector3 BoundingSphere::getCentre() const
{
	return m_center;
}

const float BoundingSphere::getRadius() const
{
	return m_radius;
}

void BoundingSphere::setCenter(Vector3 center)
{
	m_center = center;
}

void BoundingSphere::setRadius(float radius)
{
	m_radius = radius;
}

