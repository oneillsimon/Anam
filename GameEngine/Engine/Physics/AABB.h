#ifndef AABB_H
#define AABB_H

#include "../Core/Math3D.h"
#include "IntersectionData.h"

class AABB
{
private:
	const Vector3 m_minExtents;
	const Vector3 m_maxExtents;
public:
	AABB(const Vector3& minExtends, const Vector3& maxExtents);

	IntersectionData intersectAABB(const AABB& other) const;

	const Vector3& getMinExtents() const;
	const Vector3& getMaxExtents() const;
};

#endif