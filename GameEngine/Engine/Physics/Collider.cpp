#include "Collider.h"
#include "Collision.h"

Collider::Collider(int type) :
	m_type(type)
{
}

RigidBody* Collider::getBody() const
{
	return m_body;
}

int Collider::getType()
{
	return m_type;
}

void Collider::setBody(RigidBody* rigidBody)
{
	m_body = rigidBody;
}

Vector3 Collider::getAxis(unsigned index) const
{
	return m_body->getParent()->getTransform()->getTransformation().getAxisVector(index);
}

ColliderSphere::ColliderSphere(float radius) :
	Collider(Type::SPHERE),
	m_radius(radius)
{
}

void ColliderSphere::collide(Collider& collider, CollisionData& data)
{
	ColliderSphere& w = (ColliderSphere&)collider;

	switch(collider.getType())
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

Vector3 ColliderSphere::getExtents()
{
	return Vector3(m_radius, m_radius, m_radius);
}

float ColliderSphere::getRadius() const
{
	return m_radius;
}

ColliderBox::ColliderBox(const Vector3& halfExtents) :
	Collider(Type::BOX),
	m_halfSize(halfExtents)
{
}

void ColliderBox::collide(Collider& collider, CollisionData& data)
{
	switch(collider.getType())
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

Vector3 ColliderBox::getExtents()
{
	return m_halfSize;
}

Vector3 ColliderBox::getHalfSize() const
{
	return m_halfSize;
}

ColliderPlane::ColliderPlane(const Vector3& normal, float distance) :
	Collider(Type::PLANE),
	m_normal(normal),
	m_distance(distance)
{
}

void ColliderPlane::collide(Collider& collider, CollisionData& data)
{
	switch(collider.getType())
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

Vector3 ColliderPlane::getExtents()
{
	return Vector3();
}

Vector3 ColliderPlane::getNormal() const
{
	return m_normal;
}

float ColliderPlane::getDistance() const
{
	return m_distance;
}