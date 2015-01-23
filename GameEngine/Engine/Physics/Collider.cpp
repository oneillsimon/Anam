#include "Collider.h"
#include "CollisionFine.h"

ColliderSphere::ColliderSphere(float radius) :
	m_radius(radius)
{
	m_type = Type::SPHERE;
}

Vector3 ColliderSphere::getExtents()
{
	return Vector3(m_radius, m_radius, m_radius);
}

void ColliderSphere::collide(Collider& collider, CollisionData* data)
{
	switch(collider.m_type)
	{
	case BOX:
		CollisionDetector::boxAndSphere(*dynamic_cast<ColliderBox*>(&collider), *this, data);
		break;
	case SPHERE:
		CollisionDetector::sphereAndSphere(*dynamic_cast<ColliderSphere*>(&collider), *this, data);
		break;
	default:
		break;
	}
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

void ColliderBox::collide(Collider& collider, CollisionData* data)
{
	switch(collider.m_type)
	{
	case BOX:
		CollisionDetector::boxAndBox(*dynamic_cast<ColliderBox*>(&collider), *this, data);
		break;
	case SPHERE:
		CollisionDetector::boxAndSphere(*this, *dynamic_cast<ColliderSphere*>(&collider), data);
		break;
	default:
		break;
	}
}

ColliderPlane::ColliderPlane(const Vector3& direction, float offset) :
	m_direction(direction),
	m_offset(offset)
{
}

void Collider::calculateInternals()
{
}