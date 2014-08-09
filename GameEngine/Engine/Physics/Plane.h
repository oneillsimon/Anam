#ifndef PLANE_H
#define PLANE_H

#include "../Core/Math3D.h"
#include "IntersectionData.h"
#include "BoundingSphere.h"

class Plane
{
private:
	const Vector3 m_normal;
	const float m_distance;

public:
	Plane(const Vector3& normal, float distance);

	Plane normalised() const;

	IntersectionData intersectSphere(const BoundingSphere& sphere) const;

	const Vector3& getNormal() const;
	float getDistance() const;
};

#endif