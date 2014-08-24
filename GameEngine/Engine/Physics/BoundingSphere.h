#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H

#include "Collider.h"
#include "IntersectionData.h"

class BoundingSphere : public Collider
{
private:
	Vector3 m_center;
	float m_radius;

public:
	BoundingSphere(Vector3 center, float radius);

	IntersectionData intersectBoundingSphere(const BoundingSphere& other);
	virtual void transform(const Vector3& translation);

	virtual Vector3 getCentre() const;
	const float getRadius() const;

	void setCenter(Vector3 center);
	void setRadius(float radius);
};

#endif