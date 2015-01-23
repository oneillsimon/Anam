#include <assert.h>

#include "CollisionFine.h"

bool IntersectionTests::sphereAndHalfSpace(const ColliderSphere& sphere, const ColliderPlane& plane)
{
	float ballDistance = plane.m_direction.scalarProduct(sphere.getAxis(3)) - sphere.m_radius;
	return ballDistance <= plane.m_offset;
}

bool IntersectionTests::sphereAndSphere(const ColliderSphere& one, const ColliderSphere& two)
{
	Vector3 midLine = one.getAxis(3) - two.getAxis(3);

	return midLine.squareLength() < (one.m_radius + two.m_radius) * (one.m_radius + two.m_radius);
}

static float transformToAxis(const ColliderBox& box, const Vector3& axis)
{
	return box.m_halfSize.getX() * fabsf(axis.scalarProduct(box.getAxis(0))) +
		   box.m_halfSize.getY() * fabsf(axis.scalarProduct(box.getAxis(1))) +
		   box.m_halfSize.getZ() * fabsf(axis.scalarProduct(box.getAxis(2)));
}

static bool overlapOnAxis(const ColliderBox& one, const ColliderBox& two, const Vector3& axis, const Vector3& toCentre)
{
	float oneProject = transformToAxis(one, axis);
	float twoProject = transformToAxis(two, axis);

	float distance = fabsf(toCentre.scalarProduct(axis));

	return (distance < oneProject + twoProject);
}

#define TEST_OVERLAP(axis) overlapOnAxis(one, two, (axis), toCentre)

bool IntersectionTests::boxAndBox(const ColliderBox& one, const ColliderBox& two)
{
	Vector3 toCentre = two.getAxis(3) - one.getAxis(3);

	return (
		overlapOnAxis(one, two, one.getAxis(0), toCentre) &&
		overlapOnAxis(one, two, one.getAxis(1), toCentre) &&
		overlapOnAxis(one, two, one.getAxis(2), toCentre) &&

		overlapOnAxis(one, two, two.getAxis(0), toCentre) &&
		overlapOnAxis(one, two, two.getAxis(1), toCentre) &&
		overlapOnAxis(one, two, two.getAxis(2), toCentre) &&

		overlapOnAxis(one, two, two.getAxis(0) % two.getAxis(0), toCentre) &&
		overlapOnAxis(one, two, two.getAxis(0) % two.getAxis(1), toCentre) &&
		overlapOnAxis(one, two, two.getAxis(0) % two.getAxis(2), toCentre) &&

		overlapOnAxis(one, two, two.getAxis(1) % two.getAxis(0), toCentre) &&
		overlapOnAxis(one, two, two.getAxis(1) % two.getAxis(1), toCentre) &&
		overlapOnAxis(one, two, two.getAxis(1) % two.getAxis(2), toCentre) &&

		overlapOnAxis(one, two, two.getAxis(2) % two.getAxis(0), toCentre) &&
		overlapOnAxis(one, two, two.getAxis(2) % two.getAxis(1), toCentre) &&
		overlapOnAxis(one, two, two.getAxis(2) % two.getAxis(2), toCentre)
		);
}

bool IntersectionTests::boxAndHalfSpace(const ColliderBox& box, const ColliderPlane& plane)
{
	float projectedRadius = transformToAxis(box, plane.m_direction);
	float boxDistance = plane.m_direction.scalarProduct(box.getAxis(3)) - projectedRadius;

	return boxDistance <= plane.m_offset;
}

unsigned CollisionDetector::sphereAndTruePlane(const ColliderSphere& sphere, const ColliderPlane& plane, CollisionData* data)
{
	if(data->m_contactsLeft <= 0)
	{
		return 0;
	}

	Vector3 position = sphere.getAxis(3);
	float centreDistance = plane.m_direction.scalarProduct(position) - plane.m_offset;

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
	contact->setBodyData(sphere.m_body, 0, data->m_friction, data->m_restitution);

	data->addContacts(1);
	return 1;
}

unsigned CollisionDetector::sphereAndHalfSpace(const ColliderSphere& sphere, const ColliderPlane& plane, CollisionData* data)
{
	if(data->m_contactsLeft <= 0)
	{
		return 0;
	}

	Vector3 position = sphere.getAxis(3);
	float ballDistance = plane.m_direction.scalarProduct(position) - sphere.m_radius - plane.m_offset;

	if(ballDistance >= 0)
	{
		return 0;
	}

	Contact* contact = data->m_contacts;
	contact->m_contactNormal = plane.m_direction;
	contact->m_penetration = -ballDistance;
	contact->m_contactPoint = position - plane.m_direction * (ballDistance + sphere.m_radius);
	contact->setBodyData(sphere.m_body, 0, data->m_friction, data->m_restitution);

	data->addContacts(1);
	return 1;
}

unsigned CollisionDetector::sphereAndSphere(const ColliderSphere& one, const ColliderSphere& two, CollisionData* data)
{
	if(!IntersectionTests::sphereAndSphere(one, two))
	{
		return 0;
	}

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
	contact->m_penetration = (one.m_radius + two.m_radius - size);
	contact->setBodyData(one.m_body, two.m_body, data->m_friction, data->m_restitution);

	data->addContacts(1);
	return 1;
}

static float penetrationOnAxis(const ColliderBox& one, const ColliderBox& two, const Vector3& axis, const Vector3& toCentre)
{
	float oneProject = transformToAxis(one, axis);
	float twoProject = transformToAxis(one, axis);

	float distance = fabsf(toCentre.scalarProduct(axis));

	return oneProject + twoProject - distance;
}

static bool tryAxis(const ColliderBox& one, const ColliderBox& two, const Vector3& axis, const Vector3& toCentre, unsigned index, float& smallestPenetration, unsigned& smallestCase)
{
	if(axis.squareLength() < 0.0001f)
	{
		return true;
	}
	

	axis = axis.normalised();
	float penetration = penetrationOnAxis(one, two, axis, toCentre);

	if(penetration < 0)
	{
		return false;
	}

	if(penetration < smallestPenetration)
	{
		smallestPenetration = penetration;
		smallestCase = index;
	}

	return true;
}

void fillPointFaceBoxBox(const ColliderBox& one, const ColliderBox& two, const Vector3& toCentre, CollisionData* data, unsigned best, float penetration)
{
	Contact* contact = data->m_contacts;

	Vector3 normal = one.getAxis(best);

	if(one.getAxis(best).scalarProduct(toCentre) > 0)
	{
		normal = normal * -1.0f;
	}

	Vector3 vertex = two.m_halfSize;

	if(two.getAxis(0).scalarProduct(normal) < 0)
	{
		vertex.setX(vertex.getX());
	}

	if(two.getAxis(1).scalarProduct(normal) < 0)
	{
		vertex.setY(vertex.getY());
	}

	if(two.getAxis(2).scalarProduct(normal) < 0)
	{
		vertex.setZ(vertex.getZ());
	}

	contact->m_contactNormal = normal;
	contact->m_penetration = penetration;
	contact->m_contactPoint = two.m_owner->getTransformation() * vertex;
	contact->setBodyData(one.m_body, two.m_body, data->m_friction, data->m_restitution);
}

static Vector3 contactPoint(const Vector3& pOne, const Vector3& dOne, float oneSize, const Vector3& pTwo, const Vector3& dTwo, float twoSize, bool useOne)
{
	Vector3 toSt, cOne, cTwo;
	float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
	float denom, mua, mub;

	smOne = dOne.squareLength();
	smTwo = dTwo.squareLength();
	dpOneTwo = dTwo.scalarProduct(toSt);

	toSt = pOne - pTwo;
	dpStaOne = dOne.scalarProduct(toSt);
	dpStaTwo = dTwo.scalarProduct(toSt);

	denom = smOne * smTwo - dpOneTwo * dpOneTwo;

	if(fabsf(denom) < 0.0001f)
	{
		return useOne ? pOne : pTwo;
	}

	mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
	mub = (smOne * dpStaTwo - dpStaTwo - dpOneTwo * dpStaOne) / denom;

	if(mua > oneSize || mua < -oneSize ||
	   mub > twoSize || mub < -twoSize)
	{
		return useOne ? pOne : pTwo;
	}
	else
	{
		cOne = pOne + dOne * mua;
		cTwo = pTwo + dTwo * mub;

		return cOne * 0.5f + cTwo * 0.5f;
	}
}

#define checkOverlap(axis, index) \
	if(!tryAxis(one, two, (axis), toCentre, (index), penetration, best)) return 0;

unsigned CollisionDetector::boxAndBox(const ColliderBox& one, const ColliderBox& two, CollisionData* data)
{
	Vector3 toCentre = two.getAxis(3) - one.getAxis(3);

	float penetration = FLT_MAX;
	unsigned best = 0xffffff;

	checkOverlap(one.getAxis(0), 0);
	checkOverlap(one.getAxis(1), 1);
	checkOverlap(one.getAxis(2), 2);

	checkOverlap(two.getAxis(0), 3);
	checkOverlap(two.getAxis(1), 4);
	checkOverlap(two.getAxis(2), 5);

	unsigned bestSingleAxis = best;

	checkOverlap(one.getAxis(0) % two.getAxis(0), 6);
	checkOverlap(one.getAxis(0) % two.getAxis(1), 7);
	checkOverlap(one.getAxis(0) % two.getAxis(2), 8);
	checkOverlap(one.getAxis(1) % two.getAxis(0), 9);
	checkOverlap(one.getAxis(1) % two.getAxis(1), 10);
	checkOverlap(one.getAxis(1) % two.getAxis(2), 11);
	checkOverlap(one.getAxis(2) % two.getAxis(0), 12);
	checkOverlap(one.getAxis(2) % two.getAxis(1), 13);
	checkOverlap(one.getAxis(2) % two.getAxis(2), 14);

	assert(best != 0xffffff);

	if(best < 3)
	{
		fillPointFaceBoxBox(one, two, toCentre, data, best, penetration);
		data->addContacts(1);
		return 1;
	}
	else if(best < 6)
	{
		fillPointFaceBoxBox(two, one, toCentre * -1.0f, data, best - 3, penetration);
		data->addContacts(1);
		return 1;
	}
	else
	{
		best -= 6;
		unsigned oneAxisIndex = best / 3;
		unsigned twoAxisIndex = best % 3;

		Vector3 oneAxis = one.getAxis(oneAxisIndex);
		Vector3 twoAxis = two.getAxis(twoAxisIndex);
		Vector3 axis = (oneAxis % twoAxis).normalised();

		if(axis.scalarProduct(toCentre) > 0)
		{
			axis = axis * -1.0f;
		}

		Vector3 ptOnOneEdge = one.m_halfSize;
		Vector3 ptOnTwoEdge = two.m_halfSize;

		for(unsigned i = 0; i < 3; i++)
		{
			if(i == oneAxisIndex)
			{
				ptOnOneEdge[i] = 0;
			}
			else if(one.getAxis(i).scalarProduct(axis) > 0)
			{
				ptOnOneEdge[i] = -ptOnOneEdge[i];
			}

			if(i == twoAxisIndex)
			{
				ptOnTwoEdge[i] = 0;
			}
			else if(two.getAxis(i).scalarProduct(axis) < 0)
			{
				ptOnTwoEdge[i] = -ptOnTwoEdge[i];
			}
		}

		ptOnOneEdge = one.m_owner->getTransformation() * ptOnOneEdge;
		ptOnTwoEdge = two.m_owner->getTransformation() * ptOnTwoEdge;

		Vector3 vertex = contactPoint(ptOnOneEdge, oneAxis, one.m_halfSize[oneAxisIndex],
									  ptOnTwoEdge, twoAxis, two.m_halfSize[twoAxisIndex],
									  bestSingleAxis > 2);

		Contact* contact = data->m_contacts;
		contact->m_penetration = penetration;
		contact->m_contactNormal = axis;
		contact->m_contactPoint = vertex;
		contact->setBodyData(one.m_body, two.m_body, data->m_friction, data->m_restitution);
		data->addContacts(1);
		return 1;
	}

	return 0;
}

#undef checkOverlap

unsigned CollisionDetector::boxAndPoint(const ColliderBox& box, const Vector3& point, CollisionData* data)
{
	Vector3 relPt = box.m_owner->getTransformation().transformInverse(point);
	Vector3 normal;

	float minDepth = box.m_halfSize.getX() - fabsf(relPt.getX());

	if(minDepth < 0)
	{
		return 0;
	}
	
	normal = box.getAxis(0) * ((relPt.getX() < 0) ? -1.0f : 1.0f);

	float depth = box.m_halfSize.getY() - fabsf(relPt.getY());

	if(depth < 0)
	{
		return 0;
	}
	else if(depth < minDepth)
	{
		minDepth = depth;
		normal = box.getAxis(1) * ((relPt.getY() < 0) ? -1.0f : 1.0f);
	}

	depth = box.m_halfSize.getZ() - fabsf(relPt.getZ());

	if(depth < 0)
	{
		return 0;
	}
	else if(depth < minDepth)
	{
		minDepth = depth;
		normal = box.getAxis(2) * ((relPt.getZ() < 0) ? -1.0f : 1.0f);
	}

	Contact* contact = data->m_contacts;
	contact->m_contactNormal = normal;
	contact->m_contactPoint = point;
	contact->m_penetration = minDepth;
	contact->setBodyData(box.m_body, 0, data->m_friction, data->m_restitution);
	data->addContacts(1);
	return 1;
}

unsigned CollisionDetector::boxAndSphere(const ColliderBox& box, const ColliderSphere& sphere, CollisionData* data)
{
	Vector3 centre = sphere.getAxis(3);
	Vector3 relCentre = box.m_owner->getTransformation().transformInverse(centre);

	if(fabsf(relCentre.getX()) - sphere.m_radius > box.m_halfSize.getX() ||
	   fabsf(relCentre.getY()) - sphere.m_radius > box.m_halfSize.getY() ||
	   fabsf(relCentre.getZ()) - sphere.m_radius > box.m_halfSize.getZ())
	{
		return 0;
	}

	Vector3 closetPt = Vector3();
	float distance;

	for(int i = 0; i < 3; i++)
	{
		distance = relCentre[i];

		if(distance > box.m_halfSize[i])
		{
			distance = box.m_halfSize[i];
		}

		if(distance < -box.m_halfSize[i])
		{
			distance = -box.m_halfSize[i];
		}

		closetPt[i] = distance;
	}

	distance = (closetPt - relCentre).squareLength();

	if(distance > sphere.m_radius * sphere.m_radius)
	{
		return 0;
	}

	Vector3 closestPtWorld = box.m_owner->getTransformation().transform(closetPt);

	Contact* contact = data->m_contacts;
	contact->m_contactNormal = (closestPtWorld - centre);
	contact->m_contactNormal = contact->m_contactNormal.normalised();
	contact->m_contactPoint = closestPtWorld;
	contact->m_penetration = sphere.m_radius - sqrtf(distance);
	contact->setBodyData(box.m_body, sphere.m_body, data->m_friction, data->m_restitution);
	data->addContacts(1);
	return 1;
}

unsigned CollisionDetector::boxAndHalfSpace(const ColliderBox& box, const ColliderPlane& plane, CollisionData* data)
{
	if(data->m_contactsLeft <= 0)
	{
		return 0;
	}

	if(!IntersectionTests::boxAndHalfSpace(box, plane))
	{
		return 0;
	}

	static float mults[8][3] = { { +1, +1, +1 },
								 { -1, +1, +1 },
								 { +1, -1, +1 },
								 { -1, -1, 1 },
								 { +1, +1, -1 },
								 { -1, +1, -1 },
								 { +1, -1, -1 },
								 { -1, -1, -1 } };

	Contact* contact = data->m_contacts;
	unsigned contactsUsed = 0;

	for(unsigned i = 0; i < 8; i++)
	{
		Vector3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);
		vertexPos = vertexPos * box.m_halfSize;
		vertexPos = box.m_owner->getTransformation().transform(vertexPos);
		printf("box  pos: %f\n", box.m_owner->getPosition());
		float vertexDistance = vertexPos.scalarProduct(plane.m_direction);
		printf("vert dist: %f\n", vertexDistance);
		if(vertexDistance < plane.m_offset)
		{
			contact->m_contactPoint = plane.m_direction;
			contact->m_contactPoint *= (vertexDistance - plane.m_offset);
			contact->m_contactPoint += vertexPos;
			contact->m_contactNormal = plane.m_direction;
			contact->m_penetration = plane.m_offset - vertexDistance;

			contact->setBodyData(box.m_body, 0, data->m_friction, data->m_restitution);
			contact++;
			contactsUsed++;

			if(contactsUsed == (unsigned)data->m_contactsLeft)
			{
				return contactsUsed;
			}
		}
	}

	data->addContacts(contactsUsed);
	return contactsUsed;
}