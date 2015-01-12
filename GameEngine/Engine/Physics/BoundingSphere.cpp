#include "Collider.h"

BoundingSphere::BoundingSphere(float radius) :
	Collider(Collider::TYPE_SPHERE),
	m_radius(radius)
{
	m_scale = Vector3(radius, radius, radius);
}

IntersectionData BoundingSphere::intersectBoundingSphere(const BoundingSphere& other)
{
	float radiusDistance = m_radius + other.getRadius();
	Vector3 direction = (other.getCentre() - getCentre());
	float centerDistance = direction.length();
	direction /= centerDistance;

	float distance = centerDistance - radiusDistance;

	return IntersectionData(distance < 0, direction * distance);
}

IntersectionData BoundingSphere::intersectAABB(const AABB& other)
{
	Vector3 direction = other.getCentre() - getCentre();
	Vector3 closetest = direction;

	return IntersectionData(false, Vector3());
}

const float BoundingSphere::getRadius() const
{
	return m_radius;
}

void BoundingSphere::setRadius(float radius)
{
	m_radius = radius;
}

