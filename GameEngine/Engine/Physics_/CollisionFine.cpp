#include "CollideFine.h"

unsigned CollisionDetector::sphereAndSphere(const Sphere_& one, const Sphere_& two, CollisionData* data)
{
	if(data->m_contactsLeft <= 0)
	{
		return 0;
	}

	Vector3 positionOne = one.getAxis(3);
	Vector3 positionTwo = two.getAxis(3);

	Vector3 midline = positionOne - positionTwo;
	float size = midline.length();

	if(size <= 0.0f || size >= one.m_radius + two.m_radius)
	{
		return 0;
	}

	Vector3 normal = midline * (1.0f / size);

	Contact* contact = data->m_contacts;
	contact->m_contactNormal = normal;
	contact->m_contactPoint = positionOne + midline * 0.5f;
	contact->m_penetration = one.m_radius + two.m_radius - size;

	contact->m_bodies[0] = one.m_body;
	contact->m_bodies[1] = two.m_body;
	contact->m_restitution = data->m_restitution;
	contact->m_friction = data->m_friction;

	return 1;
}

unsigned CollisionDetector::sphereAndHalfSapce(const Sphere_& sphere, const Plane_& plane, CollisionData* data)
{
	if(data->m_contactsLeft <= 0)
	{
		return 0;
	}

	Vector3 position = sphere.getAxis(3);

	float sphereDistance = plane.m_direction.byVector3ToFloat(position) - sphere.m_radius - plane.m_offset;

	if(sphereDistance >= 0)
	{
		return 0;
	}

	Contact* contact = data->m_contacts;
	contact->m_contactNormal = plane.m_direction;
	contact->m_penetration = -sphereDistance;
	contact->m_contactPoint = position - plane.m_direction * (sphereDistance + sphere.m_radius);

	contact->m_bodies[0] = sphere.m_body;
	contact->m_bodies[1] = 0;
	contact->m_restitution = data->m_restitution;
	contact->m_friction = data->m_friction;

	return 1;
}

unsigned CollisionDetector::sphereAndTruePlane(const Sphere_& sphere, const Plane_& plane, CollisionData* data)
{
	if(data->m_contactsLeft <= 0)
	{
		return 0;
	}

	Vector3 position = sphere.getAxis(3);
	float centreDistance = plane.m_direction.byVector3ToFloat(position) - plane.m_offset;

	if(centreDistance * centreDistance > sphere.m_radius * sphere.m_radius)
	{
		return 0;
	}

	Vector3 normal = plane.m_direction;
	float penetration = -centreDistance;

	if(centreDistance < 0)
	{
		normal *= -1;
		penetration = -penetration;
	}

	penetration += sphere.m_radius;

	Contact* contact = data->m_contacts;
	contact->m_contactNormal = normal;
	contact->m_penetration = penetration;
	contact->m_contactPoint = position - plane.m_direction * centreDistance;

	contact->m_bodies[0] = sphere.m_body;
	contact->m_bodies[1] = 0;
	contact->m_restitution = data->m_restitution;
	contact->m_friction = data->m_friction;

	return 1;
}

unsigned CollisionDetector::boxAndSphere(const Box_& box, const Sphere_& sphere, CollisionData* data)
{
	Vector3 centre = sphere.getAxis(3);
	Vector3 relCentre = box.m_transform.transformInverse(centre);

	if(fabsf(relCentre.getX()) - sphere.m_radius > box.m_halfSize.getX() ||
	   fabsf(relCentre.getY()) - sphere.m_radius > box.m_halfSize.getY() ||
	   fabsf(relCentre.getZ()) - sphere.m_radius > box.m_halfSize.getZ())
	{
		return 0;
	}

	Vector3 closestPoint(0, 0, 0);
	float distance;

	distance = relCentre.getX();

	if(distance > box.m_halfSize.getX())
	{
		distance = box.m_halfSize.getX();
	}

	if(distance < -box.m_halfSize.getX())
	{
		distance = -box.m_halfSize.getX();
	}

	closestPoint.setX(distance);

	distance = relCentre.getY();

	if(distance > box.m_halfSize.getY())
	{
		distance = box.m_halfSize.getY();
	}

	if(distance < -box.m_halfSize.getY())
	{
		distance = -box.m_halfSize.getY();
	}

	closestPoint.setY(distance);

	distance = relCentre.getZ();

	if(distance > box.m_halfSize.getZ())
	{
		distance = box.m_halfSize.getZ();
	}

	if(distance < -box.m_halfSize.getZ())
	{
		distance = -box.m_halfSize.getZ();
	}

	closestPoint.setZ(distance);

	distance = (closestPoint - relCentre).length() * (closestPoint - relCentre).length();

	if(distance > sphere.m_radius * sphere.m_radius)
	{
		return 0;
	}

	Vector3 closestPointWorld = box.m_transform.transform(closestPoint);

	Contact* contact = data->m_contacts;
	contact->m_contactNormal = (centre - closestPointWorld);
	contact->m_contactNormal = contact->m_contactNormal.normalised();
	contact->m_contactPoint = closestPointWorld;
	contact->m_penetration = sphere.m_radius - sqrtf(distance);

	contact->m_bodies[0] = box.m_body;
	contact->m_bodies[1] = sphere.m_body;
	contact->m_restitution = data->m_restitution;
	contact->friction = data->m_friciton;

	return 1;
}

unsigned CollisionDetector::boxAndPoint(const Box_& box, const Vector3& point, CollisionData* data)
{
	Vector3 relPoint = box.m_transform.transformInverse(point);
	Vector3 normal;

	float min_depth = box.m_halfSize.getX() - fabsf(relPoint.getX());

	if(min_depth < 0)
	{
		return 0;
	}
	
	if(relPoint.getX() < 0)
	{
		normal = box.getAxis(0) * -1;
	}
	else
	{
		normal = box.getAxis(0) * 1;
	}

	float depth = box.m_halfSize.getY() - fabsf(relPoint.getY());

	if(depth < 0)
	{
		return 0;
	}
	else if(depth < min_depth)
	{
		min_depth = depth;
		
		if(relPoint.getY() < 0)
		{
			normal = box.getAxis(1) * -1;
		}
		else
		{
			normal = box.getAxis(1) * 1;
		}
	}

	depth = box.m_halfSize.getZ() - fabsf(relPoint.getZ());

	if(depth < 0)
	{
		return 0;
	}
	else if(depth < min_depth)
	{
		min_depth = depth;
		
		if(relPoint.getZ() < 0)
		{
			normal = box.getAxis(2) * -1;
		}
		else
		{
			normal = box.getAxis(2) * 1;
		}
	}

	Contact* contact = data->m_contacts;
	contact->m_contactNormal = normal;
	contact->m_contactPoint = point;
	contact->m_penetration = min_depth;

	contact->m_bodies[0] = box.m_body;
	contact->m_bodies[1] = 0;
	contact->m_restitution = data->m_restitution;
	contact->m_friction = data->m_friction;

	return 1;
}

float CollisionDetector::transformToAxis(const Box_& box, const Vector3& axis)
{
	return box.m_halfSize.getX() * fabsf(axis.byVector3ToFloat(box.getAxis(0))) +
		   box.m_halfSize.getY() * fabsf(axis.byVector3ToFloat(box.getAxis(1))) +
		   box.m_halfSize.getZ() * fabsf(axis.byVector3ToFloat(box.getAxis(2)));
}

bool CollisionDetector::overlaOnAxis(const Box_& one, const Box_& two, const Vector3& axis)
{
	float oneProject = transformToAxis(one, axis);
	float twoProject = transformToAxis(two, axis);

	Vector3 toCentre = two.getAxis(3) - one.getAxis(3);
	float distance = fabsf(toCentre.byVector3ToFloat(axis));

	return distance < oneProject + twoProject;
}

Vector3 Primitive::getAxis(unsigned int index) const
{
	return m_transform.getAxisVector(index);
}