#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H

#include "../Core/Math3D.h"
#include "IntersectionData.h"

class BoundingSphere
{
private:
	Vector3 m_center;
	float m_radius;

public:
	BoundingSphere(Vector3 center, float radius);

	IntersectionData intersectBoundingSphere(const BoundingSphere& other);

	const Vector3 getCenter() const;
	const float getRadius() const;

	void setCenter(Vector3 center);
	void setRadius(float radius);
};

#endif