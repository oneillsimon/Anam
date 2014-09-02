#include "Collider.h"

AABB::AABB(const Vector3& minExtents, const Vector3& maxExtents) :
	Collider(Collider::TYPE_AABB, "cube.obj"),
	m_minExtents(minExtents),
	m_maxExtents(maxExtents)
{
	m_scale = minExtents - maxExtents;
}

IntersectionData AABB::intersectAABB(const AABB& other) const
{
	Vector3 distances1 = other.getMinExtents() - m_maxExtents;
	Vector3 distances2 = m_minExtents - other.getMaxExtents();
	Vector3 distances = distances1.max(distances2);

	float maxDistance = distances.max();

	return IntersectionData(maxDistance < 0, distances);
}

const Vector3& AABB::getMinExtents() const
{
	return m_minExtents;
}

const Vector3& AABB::getMaxExtents() const
{
	return m_maxExtents;
}