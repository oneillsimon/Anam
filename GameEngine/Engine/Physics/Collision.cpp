#include "Collision.h"

bool overlapOnAxis(const ColliderBox& one, const ColliderBox& two, const Vector3& axis, const Vector3& toCentre);
float transformToAxis(const ColliderBox& box, const Vector3& axis);
float penetrationOnAxis(const ColliderBox& one, const ColliderBox& two, const Vector3& axis, const Vector3& toCentre);
bool tryAxis(const ColliderBox& one, const ColliderBox& two, Vector3 axis, const Vector3& toCentre, unsigned index, float& smallestPenetration, unsigned& smallestCase);
void fillPointFaceBoxBox(const ColliderBox& one, const ColliderBox &two, const Vector3& toCentre, CollisionData* data, unsigned best, float pen);
Vector3 contactPoint(const Vector3& pOne, const Vector3& dOne, float oneSize, const Vector3& pTwo, const Vector3& dTwo, float twoSize, bool useOne);

unsigned CollisionDetector::sphereAndHalfSpace(const ColliderSphere& sphere, const ColliderPlane& plane, CollisionData* data)
{
	if(data->getContactsLeft() <= 0)
	{
		return 0;
	}

	Vector3 position = sphere.getAxis(3);

	float ballDistance = plane.getNormal().scalarProduct(position) - sphere.getRadius() - plane.getDistance();

	if(ballDistance >= 0)
	{
		return 0;
	}

	Contact* contact = data->getContacts();
	contact->setContactNormal(plane.getNormal());
	contact->setPenetration(-ballDistance);
	contact->setContactPoint(position - plane.getNormal() * (ballDistance + sphere.getRadius()));
	contact->setBodyData(sphere.getBody(), 0, data->getFriction(), data->getRestitution());
	data->addContacts(1);

	return 1;
}

unsigned CollisionDetector::sphereAndSphere(const ColliderSphere& one, const ColliderSphere& two, CollisionData* data)
{
	if(data->getContactsLeft() <= 0)
	{
		return 0;
	}

	Vector3 positionOne = one.getAxis(3);
	Vector3 positionTwo = two.getAxis(3);

	Vector3 midline = positionOne - positionTwo;
	float size = midline.length();

	if(size <= 0.0f || size >= one.getRadius() + two.getRadius())
	{
		return 0;
	}

	Vector3 normal = midline * (1.0f / size);

	Contact* contact = data->getContacts();
	contact->setContactNormal(normal);
	contact->setContactPoint(positionOne + midline * 0.5f);
	contact->setPenetration(one.getRadius() + two.getRadius() - size);
	contact->setBodyData(one.getBody(), two.getBody(), data->getFriction(), data->getRestitution());

	data->addContacts(1);

	return 1;
}

#define CHECK_OVERLAP(axis, index) \
    if (!tryAxis(one, two, (axis), toCentre, (index), pen, best)) return 0;

unsigned CollisionDetector::boxAndBox(const ColliderBox& one, const ColliderBox& two, CollisionData* data)
{
	//if(!IntersectionTests::boxAndBox(one, two))
	//{
	//	return 0;
	//}

	Vector3 toCentre = two.getAxis(3) - one.getAxis(3);

	float pen = FLT_MAX;
	unsigned best = 0xffffff;

	CHECK_OVERLAP(one.getAxis(0), 0);
	CHECK_OVERLAP(one.getAxis(1), 1);
	CHECK_OVERLAP(one.getAxis(2), 2);

	CHECK_OVERLAP(two.getAxis(0), 3);
	CHECK_OVERLAP(two.getAxis(1), 4);
	CHECK_OVERLAP(two.getAxis(2), 5);

	unsigned bestSingleAxis = best;

	CHECK_OVERLAP(one.getAxis(0) % two.getAxis(0), 6);
	CHECK_OVERLAP(one.getAxis(0) % two.getAxis(1), 7);
	CHECK_OVERLAP(one.getAxis(0) % two.getAxis(2), 8);
	CHECK_OVERLAP(one.getAxis(1) % two.getAxis(0), 9);
	CHECK_OVERLAP(one.getAxis(1) % two.getAxis(1), 10);
	CHECK_OVERLAP(one.getAxis(1) % two.getAxis(2), 11);
	CHECK_OVERLAP(one.getAxis(2) % two.getAxis(0), 12);
	CHECK_OVERLAP(one.getAxis(2) % two.getAxis(1), 13);
	CHECK_OVERLAP(one.getAxis(2) % two.getAxis(2), 14);

	assert(best != 0xffffff);

	if(best < 3)
	{
		fillPointFaceBoxBox(one, two, toCentre, data, best, pen);
		data->addContacts(1);

		return 1;
	}
	else if(best < 6)
	{
		fillPointFaceBoxBox(two, one, toCentre*-1.0f, data, best - 3, pen);
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
		Vector3 axis = oneAxis % twoAxis;
		axis = axis.normalised();

		if(axis.scalarProduct(toCentre) > 0)
		{
			axis = axis * -1.0f;
		}

		Vector3 ptOnOneEdge = one.getHalfSize();
		Vector3 ptOnTwoEdge = two.getHalfSize();

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

		ptOnOneEdge = one.getBody()->getParent()->getTransform()->getTransformation() * ptOnOneEdge;
		ptOnTwoEdge = two.getBody()->getParent()->getTransform()->getTransformation() * ptOnTwoEdge;

		Vector3 vertex = contactPoint(ptOnOneEdge,
									  oneAxis,
									  one.getHalfSize()[oneAxisIndex],
									  ptOnTwoEdge,
									  twoAxis,
									  two.getHalfSize()[twoAxisIndex],
									  bestSingleAxis > 2);

		Contact* contact = data->getContacts();
		contact->setPenetration(pen);
		contact->setContactNormal(axis);
		contact->setContactPoint(vertex);
		contact->setBodyData(one.getBody(), two.getBody(), data->getFriction(), data->getRestitution());
		data->addContacts(1);

		return 1;
	}

	return 0;
}
#undef CHECK_OVERLAP

unsigned CollisionDetector::boxAndPoint(const ColliderBox& box, const Vector3& point, CollisionData* data)
{
	Vector3 relPt = box.getBody()->getParent()->getTransform()->getTransformation().transformInverse(point);

	Vector3 normal;
	float min_depth = box.getHalfSize()[0] - fabsf(relPt[0]);

	if(min_depth < 0)
	{
		return 0;
	}

	normal = box.getAxis(0) * ((relPt[0] < 0) ? -1 : 1);

	float depth = box.getHalfSize()[1] - fabsf(relPt[1]);

	if(depth < 0)
	{
		return 0;
	}
	else if(depth < min_depth)
	{
		min_depth = depth;
		normal = box.getAxis(1) * ((relPt[1] < 0) ? -1 : 1);
	}

	depth = box.getHalfSize()[2] - fabsf(relPt[2]);

	if(depth < 0)
	{
		return 0;
	}
	else if(depth < min_depth)
	{
		min_depth = depth;
		normal = box.getAxis(2) * ((relPt[2] < 0) ? -1 : 1);
	}

	Contact* contact = data->getContacts();
	contact->setContactNormal(normal);
	contact->setContactPoint(point);
	contact->setPenetration(min_depth);
	contact->setBodyData(box.getBody(), 0, data->getFriction(), data->getRestitution());
	data->addContacts(1);

	return 1;
}

unsigned CollisionDetector::boxAndSphere(const ColliderBox& box, const ColliderSphere& sphere, CollisionData* data)
{
	Vector3 centre = sphere.getAxis(3);
	Vector3 relCentre = box.getBody()->getParent()->getTransform()->getTransformation().transformInverse(centre);

	if(fabsf(relCentre[0]) - sphere.getRadius() > box.getHalfSize()[0] ||
	   fabsf(relCentre[1]) - sphere.getRadius() > box.getHalfSize()[1] ||
	   fabsf(relCentre[2]) - sphere.getRadius() > box.getHalfSize()[2])
	{
		return 0;
	}

	Vector3 closestPt(0, 0, 0);
	float dist = relCentre[0];

	if(dist > box.getHalfSize()[0])
	{
		dist = box.getHalfSize()[0];
	}

	if(dist < -box.getHalfSize()[0])
	{
		dist = -box.getHalfSize()[0];
	}

	closestPt[0] = dist;
	dist = relCentre[1];

	if(dist > box.getHalfSize()[1])
	{
		dist = box.getHalfSize()[1];
	}

	if(dist < -box.getHalfSize()[1])
	{
		dist = -box.getHalfSize()[1];
	}

	closestPt[1] = dist;
	dist = relCentre[2];

	if(dist > box.getHalfSize()[2])
	{
		dist = box.getHalfSize()[2];
	}

	if(dist < -box.getHalfSize()[2])
	{
		dist = -box.getHalfSize()[2];
	}

	closestPt[2] = dist;

	dist = (closestPt - relCentre).squareLength();

	if(dist > sphere.getRadius() * sphere.getRadius())
	{
		return 0;
	}

	Vector3 closestPtWorld = box.getBody()->getParent()->getTransform()->getTransformation().transform(closestPt);

	Contact* contact = data->getContacts();
	contact->setContactNormal(closestPtWorld - centre);
	contact->setContactNormal(contact->getContactNormal().normalised());
	contact->setContactPoint(closestPtWorld);
	contact->setPenetration(sphere.getRadius() - sqrtf(dist));
	contact->setBodyData(box.getBody(), sphere.getBody(), data->getFriction(), data->getRestitution());
	data->addContacts(1);

	return 1;
}

unsigned CollisionDetector::boxAndHalfSpace(const ColliderBox& box, const ColliderPlane& plane, CollisionData* data)
{
	if(data->getContactsLeft() <= 0)
	{
		return 0;
	}

	if(!IntersectionTests::boxAndHalfSpace(box, plane))
	{
		return 0;
	}

	float mults[8][3] = { { 1, 1,  1 }, { -1, 1,  1 }, { 1, -1,  1 }, { -1, -1,  1 },
						  { 1, 1, -1 }, { -1, 1, -1 }, { 1, -1, -1 }, { -1, -1, -1 } };

	Contact* contact = data->getContacts();
	unsigned contactsUsed = 0;

	for(unsigned i = 0; i < 8; i++)
	{

		Vector3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);
		vertexPos *= box.getHalfSize();

		vertexPos += box.getBody()->getPosition();

		float vertexDistance = vertexPos.scalarProduct(plane.getNormal());

		if(vertexDistance <= plane.getDistance())
		{
			contact->setContactPoint(plane.getNormal());
			contact->setContactPoint(contact->getContactPoint() * (vertexDistance - plane.getDistance()));
			contact->setContactPoint(contact->getContactPoint() + vertexPos);
			contact->setContactNormal(plane.getNormal());
			contact->setPenetration(plane.getDistance() - vertexDistance);

			contact->setBodyData(box.getBody(), 0, data->getFriction(), data->getRestitution());

			contact++;
			contactsUsed++;

			if(contactsUsed == (unsigned)data->getContactsLeft())
			{
				return contactsUsed;
			}
		}
	}

	data->addContacts(contactsUsed);

	return contactsUsed;
}

#define TEST_OVERLAP(axis) overlapOnAxis(one, two, (axis), toCentre)

bool IntersectionTests::boxAndBox(const ColliderBox& one, const ColliderBox& two)
{
	Vector3 toCentre = two.getAxis(3) - one.getAxis(3);

	return (TEST_OVERLAP(one.getAxis(0)) &&
			TEST_OVERLAP(one.getAxis(1)) &&
			TEST_OVERLAP(one.getAxis(2)) &&

			TEST_OVERLAP(two.getAxis(0)) &&
			TEST_OVERLAP(two.getAxis(1)) &&
			TEST_OVERLAP(two.getAxis(2)) &&

			TEST_OVERLAP(one.getAxis(0) % two.getAxis(0)) &&
			TEST_OVERLAP(one.getAxis(0) % two.getAxis(1)) &&
			TEST_OVERLAP(one.getAxis(0) % two.getAxis(2)) &&
			TEST_OVERLAP(one.getAxis(1) % two.getAxis(0)) &&
			TEST_OVERLAP(one.getAxis(1) % two.getAxis(1)) &&
			TEST_OVERLAP(one.getAxis(1) % two.getAxis(2)) &&
			TEST_OVERLAP(one.getAxis(2) % two.getAxis(0)) &&
			TEST_OVERLAP(one.getAxis(2) % two.getAxis(1)) &&
			TEST_OVERLAP(one.getAxis(2) % two.getAxis(2)));
}
#undef TEST_OVERLAP

bool IntersectionTests::boxAndHalfSpace(const ColliderBox& box, const ColliderPlane& plane)
{
	float projectedRadius = transformToAxis(box, plane.getNormal());
	float boxDistance = plane.getNormal().scalarProduct(box.getAxis(3)) - projectedRadius;

	return boxDistance <= plane.getDistance();
}

bool CollisionData::hasMoreContacts()
{
	return m_contactsLeft > 0;
}

void CollisionData::reset(unsigned maxContacts)
{
	m_contactsLeft = maxContacts;
	m_contactCount = 0;
	m_contacts = m_contactArray;
}

void CollisionData::addContacts(unsigned count)
{
	m_contactsLeft -= count;
	m_contactCount += count;
	m_contacts += count;
}

Contact* CollisionData::getContactArray()
{
	return m_contactArray;
}

Contact* CollisionData::getContacts()
{
	return m_contacts;
}

int CollisionData::getContactsLeft()
{
	return m_contactsLeft;
}

unsigned CollisionData::getContactCount()
{
	return m_contactCount;
}

float CollisionData::getFriction()
{
	return m_friction;
}

float CollisionData::getRestitution()
{
	return m_restitution;
}

float CollisionData::getTolerance()
{
	return m_tolerance;
}

void CollisionData::setContactArray(Contact* contactArray)
{
	m_contactArray = contactArray;
}

void CollisionData::setFriction(float friction)
{
	m_friction = friction;
}

void CollisionData::setRestitution(float restitution)
{
	m_restitution = restitution;
}

void CollisionData::setTolerance(float tolerance)
{
	m_tolerance = tolerance;
}

float transformToAxis(const ColliderBox& box, const Vector3& axis)
{
	return box.getHalfSize()[0] * fabsf(axis.scalarProduct(box.getAxis(0))) +
		   box.getHalfSize()[1] * fabsf(axis.scalarProduct(box.getAxis(1))) +
		   box.getHalfSize()[2] * fabsf(axis.scalarProduct(box.getAxis(2)));
}

float penetrationOnAxis(const ColliderBox& one, const ColliderBox& two, const Vector3& axis, const Vector3& toCentre)
{
	float oneProject = transformToAxis(one, axis);
	float twoProject = transformToAxis(two, axis);
	float distance = fabsf(toCentre.scalarProduct(axis));

	return oneProject + twoProject - distance;
}


bool tryAxis(const ColliderBox& one, const ColliderBox& two, Vector3 axis, const Vector3& toCentre, unsigned index, float& smallestPenetration, unsigned& smallestCase)
{
	if(axis.squareLength() < 0.0001)
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

void fillPointFaceBoxBox(const ColliderBox& one, const ColliderBox& two, const Vector3& toCentre, CollisionData* data, unsigned best, float pen)
{
	Contact* contact = data->getContacts();

	Vector3 normal = one.getAxis(best);

	if(one.getAxis(best).scalarProduct(toCentre) > 0)
	{
		normal = normal * -1.0f;
	}

	Vector3 vertex = two.getHalfSize();

	if(two.getAxis(0).scalarProduct(normal) < 0)
	{
		vertex[0] = -vertex[0];
	}

	if(two.getAxis(1).scalarProduct(normal) < 0)
	{
		vertex[1] = -vertex[1];
	}

	if(two.getAxis(2).scalarProduct(normal) < 0)
	{
		vertex[2] = -vertex[2];
	}

	contact->setContactNormal(normal);
	contact->setPenetration(pen);
	contact->setContactPoint(two.getBody()->getParent()->getTransform()->getTransformation() * vertex);
	contact->setBodyData(one.getBody(), two.getBody(), data->getFriction(), data->getRestitution());
}

Vector3 contactPoint(const Vector3& pOne, const Vector3& dOne, float oneSize, const Vector3& pTwo, const Vector3& dTwo, float twoSize, bool useOne)
{
	Vector3 toSt, cOne, cTwo;
	float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
	float denom, mua, mub;

	smOne = dOne.squareLength();
	smTwo = dTwo.squareLength();
	dpOneTwo = dTwo.scalarProduct(dOne);

	toSt = pOne - pTwo;
	dpStaOne = dOne.scalarProduct(toSt);
	dpStaTwo = dTwo.scalarProduct(toSt);

	denom = smOne * smTwo - dpOneTwo * dpOneTwo;

	if(fabsf(denom) < 0.0001f)
	{
		return useOne ? pOne : pTwo;
	}

	mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
	mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

	if(mua >  oneSize ||
	   mua < -oneSize ||
	   mub >  twoSize ||
	   mub < -twoSize)
	{
		return useOne ? pOne : pTwo;
	}
	else
	{
		cOne = pOne + dOne * mua;
		cTwo = pTwo + dTwo * mub;

		return cOne * 0.5 + cTwo * 0.5;
	}
}

inline bool overlapOnAxis(const ColliderBox& one, const ColliderBox& two, const Vector3& axis, const Vector3& toCentre)
{
	return penetrationOnAxis(one, two, axis, toCentre) > 0;
}