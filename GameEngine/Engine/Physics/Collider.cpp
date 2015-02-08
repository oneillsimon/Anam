#include "Collider.h"
#include "Collision.h"
#include <iostream>

void Collider::collide(PhysicsObject& p1, CollisionData_& data)
{
	if(m_isColliding)
	{
		CollisionTester::addCollisionImpulse(*m_parent, p1, data);
	}
}

ColliderSphere::ColliderSphere(float radius) :
	m_radius(radius)
{
	m_type = Type::SPHERE;
}

void ColliderSphere::collide(PhysicsObject& p1, CollisionData_& data)
{
	m_isColliding = false;

	switch(p1.getCollider()->m_type)
	{
	case SPHERE:
		m_isColliding = CollisionTester::sphereAndSphere(*m_parent, p1, &data);
		break;
	case BOX:
		//CollisionTester::
		break;
	case PLANE:
		m_isColliding = CollisionTester::planeAndSphere(p1, *m_parent, &data);
		break;
	}

	Collider::collide(p1, data);
}

ColliderBox::ColliderBox(const Vector3& halfExtents) :
	m_halfSize(halfExtents)
{
	m_type = Type::BOX;
}

void ColliderBox::collide(PhysicsObject& p1, CollisionData_& data)
{
	m_isColliding = false;

	switch(p1.getCollider()->m_type)
	{
	case SPHERE:
		//m_isColliding = CollisionTester::b(*m_parent, p1);
		break;
	case BOX:
		m_isColliding = CollisionTester::boxAndBox(*m_parent, p1, &data);
		break;
	case PLANE:
		//m_isColliding = CollisionTester::planeAndSphere(p1, *m_parent);
		break;
	}

	Collider::collide(p1, data);
}

ColliderPlane::ColliderPlane(const Vector3& normal, float distance) :
	m_normal(normal),
	m_distance(distance)
{
	m_type = Type::PLANE;
}

void ColliderPlane::collide(PhysicsObject& p1, CollisionData_& data)
{
	m_isColliding = false;

	switch(p1.getCollider()->m_type)
	{
	case SPHERE:
		m_isColliding = CollisionTester::planeAndSphere(*m_parent, p1, &data);
		break;
	case BOX:
		//CollisionTester::boxAndBox(*m_parent, p1, &data);
		break;
	case PLANE:
		//m_isColliding = CollisionTester::planeAndSphere(p1, *m_parent);
		break;
	}

	Collider::collide(p1, data);
}

ColliderOBB::ColliderOBB(const Vector3& halfExtents) :
	e(halfExtents)
{
	m_type = Type::OBB;
}

void ColliderOBB::collide(PhysicsObject& p1, CollisionData_& data)
{
	m_isColliding = false;

	switch(p1.getCollider()->m_type)
	{
	case OBB:
		m_isColliding = CollisionTester::oBBAndOBB(*m_parent, p1, &data);
	default:
		break;
	}

	Collider::collide(p1, data);
}