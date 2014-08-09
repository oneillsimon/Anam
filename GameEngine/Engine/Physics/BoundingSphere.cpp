#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(Vector3 center, float radius)
{
	m_center = center;
	m_radius = radius;
}

IntersectionData BoundingSphere::intersectBoundingSphere(const BoundingSphere& other)
{
	float radiusDistance = m_radius + other.getRadius();
	float centerDistance = (other.getCenter() - m_center).length();
	float distance = centerDistance - radiusDistance;

	return IntersectionData(centerDistance < radiusDistance, distance);
}

const Vector3 BoundingSphere::getCenter() const
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

