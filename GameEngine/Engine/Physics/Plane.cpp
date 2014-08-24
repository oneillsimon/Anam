#include <math.h>
#include "Plane.h"

Plane::Plane(const Vector3& normal, float distance) :
	m_normal(normal),
	m_distance(distance)
{
}

Plane Plane::normalised() const
{
	float magnitude = m_normal.length();

	return Plane(m_normal / magnitude, m_distance / magnitude);
}

IntersectionData Plane::intersectSphere(const BoundingSphere& sphere) const
{
	float distanceFromSphereCenter = fabs(m_normal.dot(sphere.getCentre()) + m_distance);
	float distanceFromSphere = distanceFromSphereCenter - sphere.getRadius();

	return IntersectionData(distanceFromSphere < 0, m_normal * distanceFromSphere);
}

const Vector3& Plane::getNormal() const
{
	return m_normal;
}

float Plane::getDistance() const
{
	return m_distance;
}