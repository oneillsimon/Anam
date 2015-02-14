#include "Collider.h"
#include "Collision.h"
#include <iostream>

Vector3 Collider::getAxis(unsigned index) const
{
	return m_body->m_parent->getTransform()->getTransformation().getAxisVector(index);
}

ColliderSphere::ColliderSphere(float radius) :
	m_radius(radius)
{
	m_type = Type::SPHERE;
}

void ColliderSphere::collide(Collider& collider, CollisionData& data)
{
	ColliderSphere& w = (ColliderSphere&)collider;

	switch(collider.m_type)
	{
	case SPHERE:
		CollisionDetector::sphereAndSphere(*this, (ColliderSphere&)collider, &data);
		break;
	case BOX:
		CollisionDetector::boxAndSphere((ColliderBox&)collider, *this, &data);
		break;
	case PLANE:
		CollisionDetector::sphereAndHalfSpace(*this, (ColliderPlane&)collider, &data);
		break;
	}
}

ColliderBox::ColliderBox(const Vector3& halfExtents) :
	m_halfSize(halfExtents)
{
	m_type = Type::BOX;
}

void ColliderBox::collide(Collider& collider, CollisionData& data)
{
	switch(collider.m_type)
	{
	case SPHERE:
		CollisionDetector::boxAndSphere(*this, (ColliderSphere&)collider, &data);
		break;
	case BOX:
		CollisionDetector::boxAndBox(*this, (ColliderBox&)collider, &data);
		break;
	case PLANE:
		CollisionDetector::boxAndHalfSpace(*this, (ColliderPlane&)collider, &data);
		break;
	}
}

ColliderPlane::ColliderPlane(const Vector3& normal, float distance) :
	m_normal(normal),
	m_distance(distance)
{
	m_type = Type::PLANE;
}

void ColliderPlane::collide(Collider& collider, CollisionData& data)
{
	switch(collider.m_type)
	{
	case SPHERE:
		CollisionDetector::sphereAndHalfSpace((ColliderSphere&)collider, *this, &data);
		break;
	case BOX:
		CollisionDetector::boxAndHalfSpace((ColliderBox&)collider, *this, &data);
		break;
	case PLANE:
		break;
	}
}