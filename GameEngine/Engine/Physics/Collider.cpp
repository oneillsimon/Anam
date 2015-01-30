#include "Collider.h"
#include "Collision.h"

void Collider::collide(PhysicsObject& p0, PhysicsObject& p1)
{
	int type0 = p0.getCollider()->m_type;
	int type1 = p1.getCollider()->m_type;
	CollisionData_ data_;

	switch(type0)
	{
		case BOX:
			if(type1 == BOX)
			{
				if(CollisionTester::boxAndBox(p0, p1, &data_))
				{
					CollisionTester::addCollisionImpulse(p0, p1, data_);
				}
			}
			else if(type1 == SPHERE)
			{
				
			}
			break;
		case SPHERE:
			if(type1 == BOX)
			{
				//CollisionTester::boxAndBox(p0, p1);
			}
			else if(type1 == SPHERE)
			{
				if(CollisionTester::sphereAndSphere(p0, p1, &data_))
				{
					CollisionTester::addCollisionImpulse(p0, p1, data_);
				}
			}
			break;
	}
}

ColliderSphere::ColliderSphere(float radius) :
	m_radius(radius)
{
	m_type = Type::SPHERE;
}

ColliderBox::ColliderBox(const Vector3& halfExtents) :
	m_halfSize(halfExtents)
{
	m_type = Type::BOX;
}

ColliderPlane::ColliderPlane(const Vector3& direction, float offset) :
	m_direction(direction),
	m_offset(offset)
{
}
