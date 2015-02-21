#include <assert.h>

#include "Collision.h"

unsigned CollisionDetector::sphereAndHalfSpace(
	const ColliderSphere &sphere,
	const ColliderPlane &plane,
	CollisionData *data
	)
{
	// Make sure we have contacts
	if(data->m_contactsLeft <= 0) return 0;

	// Cache the sphere position
	Vector3 position = sphere.getAxis(3);

	// Find the distance from the plane
	float ballDistance =
		plane.m_normal.scalarProduct(position) -
		sphere.m_radius - plane.m_distance;

	if(ballDistance >= 0) return 0;

	// Create the contact - it has a normal in the plane direction.
	Contact* contact = data->m_contacts;
	contact->m_contactNormal = plane.m_normal;
	contact->m_penetration = -ballDistance;
	contact->m_contactPoint =
		position - plane.m_normal * (ballDistance + sphere.m_radius);
	contact->setBodyData(sphere.m_body, NULL,
		data->m_friction, data->m_restitution);

	data->addContacts(1);
	return 1;
}

unsigned CollisionDetector::sphereAndSphere(
	const ColliderSphere &one,
	const ColliderSphere &two,
	CollisionData *data
	)
{
	// Make sure we have contacts
	if(data->m_contactsLeft <= 0) return 0;

	// Cache the sphere positions
	Vector3 positionOne = one.getAxis(3);
	Vector3 positionTwo = two.getAxis(3);

	// Find the vector between the objects
	Vector3 midline = positionOne - positionTwo;
	float size = midline.length();

	// See if it is large enough.
	if(size <= 0.0f || size >= one.m_radius + two.m_radius)
	{
		return 0;
	}

	// We manually create the normal, because we have the
	// size to hand.
	Vector3 normal = midline * (1.0f / size);

	Contact* contact = data->m_contacts;
	contact->m_contactNormal = normal;
	contact->m_contactPoint = positionOne + midline * 0.5f;
	contact->m_penetration = (one.m_radius + two.m_radius - size);
	contact->setBodyData(one.m_body, two.m_body,
		data->m_friction, data->m_restitution);

	data->addContacts(1);
	return 1;
}

static float transformToAxis(
	const ColliderBox &box,
	const Vector3 &axis
	)
{
	return
		box.m_halfSize[0] * fabsf(axis.scalarProduct(box.getAxis(0))) +
		box.m_halfSize[1] * fabsf(axis.scalarProduct(box.getAxis(1))) +
		box.m_halfSize[2] * fabsf(axis.scalarProduct(box.getAxis(2)));
}

/*
* This function checks if the two boxes overlap
* along the given axis, returning the ammount of overlap.
* The final parameter toCentre
* is used to pass in the vector between the boxes centre
* points, to avoid having to recalculate it each time.
*/
static float penetrationOnAxis(
	const ColliderBox &one,
	const ColliderBox &two,
	const Vector3 &axis,
	const Vector3 &toCentre
	)
{
	// Project the half-size of one onto axis
	float oneProject = transformToAxis(one, axis);
	float twoProject = transformToAxis(two, axis);

	// Project this onto the axis
	float distance = fabsf(toCentre.scalarProduct(axis));

	// Return the overlap (i.e. positive indicates
	// overlap, negative indicates separation).
	return oneProject + twoProject - distance;
}


static bool tryAxis(
	const ColliderBox &one,
	const ColliderBox &two,
	Vector3 axis,
	const Vector3& toCentre,
	unsigned index,

	// These values may be updated
	float& smallestPenetration,
	unsigned &smallestCase
	)
{
	// Make sure we have a normalized axis, and don't check almost parallel axes
	if(axis.squareLength() < 0.0001) return true;
	axis = axis.normalised();

	float penetration = penetrationOnAxis(one, two, axis, toCentre);

	if(penetration < 0) return false;
	if(penetration < smallestPenetration) {
		smallestPenetration = penetration;
		smallestCase = index;
	}
	return true;
}

void fillPointFaceBoxBox(
	const ColliderBox &one,
	const ColliderBox &two,
	const Vector3 &toCentre,
	CollisionData *data,
	unsigned best,
	float pen
	)
{
	// This method is called when we know that a vertex from
	// box two is in contact with box one.

	Contact* contact = data->m_contacts;

	// We know which axis the collision is on (i.e. best),
	// but we need to work out which of the two faces on
	// this axis.
	Vector3 normal = one.getAxis(best);
	if(one.getAxis(best).scalarProduct(toCentre) > 0)
	{
		normal = normal * -1.0f;
	}

	// Work out which vertex of box two we're colliding with.
	// Using toCentre doesn't work!
	//Vector3 vertex = two.m_halfSize;
	Vector3 vertex = toCentre;
	if(two.getAxis(0).scalarProduct(normal) < 0) vertex[0] = -vertex[0];
	if(two.getAxis(1).scalarProduct(normal) < 0) vertex[1] = -vertex[1];
	if(two.getAxis(2).scalarProduct(normal) < 0) vertex[2] = -vertex[2];

	//vertex *= 0.5f;

	// Create the contact data
	contact->m_contactNormal = normal;
	contact->m_penetration = pen;
	contact->m_contactPoint = two.m_body->m_parent->getTransform()->getPosition() - one.m_body->m_parent->getTransform()->getPosition();
	//contact->m_contactPoint = vertex;//two.m_body->m_parent->getTransform()->getTransformation() * vertex;
	contact->setBodyData(one.m_body, two.m_body,
		data->m_friction, data->m_restitution);
}

static Vector3 contactPoint(
	const Vector3 &pOne,
	const Vector3 &dOne,
	float oneSize,
	const Vector3 &pTwo,
	const Vector3 &dTwo,
	float twoSize,

	// If this is true, and the contact point is outside
	// the edge (in the case of an edge-face contact) then
	// we use one's midpoint, otherwise we use two's.
	bool useOne)
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

	// Zero denominator indicates parrallel lines
	if(fabsf(denom) < 0.0001f) {
		return useOne ? pOne : pTwo;
	}

	mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
	mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

	// If either of the edges has the nearest point out
	// of bounds, then the edges aren't crossed, we have
	// an edge-face contact. Our point is on the edge, which
	// we know from the useOne parameter.
	if(mua > oneSize ||
		mua < -oneSize ||
		mub > twoSize ||
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

// This preprocessor definition is only used as a convenience
// in the boxAndBox contact generation method.
#define CHECK_OVERLAP(axis, index) \
    if (!tryAxis(one, two, (axis), toCentre, (index), pen, best)) return 0;

unsigned CollisionDetector::boxAndBox(
	const ColliderBox &one,
	const ColliderBox &two,
	CollisionData *data
	)
{
	//if (!IntersectionTests::boxAndBox(one, two)) return 0;

	// Find the vector between the two centres
	Vector3 toCentre = two.getAxis(3) - one.getAxis(3);

	// We start assuming there is no contact
	float pen = FLT_MAX;
	unsigned best = 0xffffff;

	// Now we check each axes, returning if it gives us
	// a separating axis, and keeping track of the axis with
	// the smallest penetration otherwise.
	CHECK_OVERLAP(one.getAxis(0), 0);
	CHECK_OVERLAP(one.getAxis(1), 1);
	CHECK_OVERLAP(one.getAxis(2), 2);

	CHECK_OVERLAP(two.getAxis(0), 3);
	CHECK_OVERLAP(two.getAxis(1), 4);
	CHECK_OVERLAP(two.getAxis(2), 5);

	// Store the best axis-major, in case we run into almost
	// parallel edge collisions later
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

	// Make sure we've got a result.
	assert(best != 0xffffff);

	// We now know there's a collision, and we know which
	// of the axes gave the smallest penetration. We now
	// can deal with it in different ways depending on
	// the case.
	if(best < 3)
	{
		// We've got a vertex of box two on a face of box one.
		fillPointFaceBoxBox(one, two, toCentre, data, best, pen);
		data->addContacts(1);
		return 1;
	}
	else if(best < 6)
	{
		// We've got a vertex of box one on a face of box two.
		// We use the same algorithm as above, but swap around
		// one and two (and therefore also the vector between their
		// centres).
		fillPointFaceBoxBox(two, one, toCentre*-1.0f, data, best - 3, pen);
		data->addContacts(1);
		return 1;
	}
	else
	{
		// We've got an edge-edge contact. Find out which axes
		best -= 6;
		unsigned oneAxisIndex = best / 3;
		unsigned twoAxisIndex = best % 3;
		Vector3 oneAxis = one.getAxis(oneAxisIndex);
		Vector3 twoAxis = two.getAxis(twoAxisIndex);
		Vector3 axis = oneAxis % twoAxis;
		axis = axis.normalised();

		// The axis should point from box one to box two.
		if(axis.scalarProduct(toCentre) > 0) axis = axis * -1.0f;

		// We have the axes, but not the edges: each axis has 4 edges parallel
		// to it, we need to find which of the 4 for each object. We do
		// that by finding the point in the centre of the edge. We know
		// its component in the direction of the box's collision axis is zero
		// (its a mid-point) and we determine which of the extremes in each
		// of the other axes is closest.
		Vector3 ptOnOneEdge = one.m_halfSize;
		Vector3 ptOnTwoEdge = two.m_halfSize;
		for(unsigned i = 0; i < 3; i++)
		{
			if(i == oneAxisIndex) ptOnOneEdge[i] = 0;
			else if(one.getAxis(i).scalarProduct(axis) > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];

			if(i == twoAxisIndex) ptOnTwoEdge[i] = 0;
			else if(two.getAxis(i).scalarProduct(axis) < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
		}

		// Move them into world coordinates (they are already oriented
		// correctly, since they have been derived from the axes).
		ptOnOneEdge = one.m_body->m_parent->getTransform()->getTransformation() * ptOnOneEdge;
		ptOnTwoEdge = two.m_body->m_parent->getTransform()->getTransformation() * ptOnTwoEdge;

		// So we have a point and a direction for the colliding edges.
		// We need to find out point of closest approach of the two
		// line-segments.
		Vector3 vertex = contactPoint(
			ptOnOneEdge, oneAxis, one.m_halfSize[oneAxisIndex],
			ptOnTwoEdge, twoAxis, two.m_halfSize[twoAxisIndex],
			bestSingleAxis > 2
			);

		// We can fill the contact.
		Contact* contact = data->m_contacts;

		contact->m_penetration = pen;
		contact->m_contactNormal = axis;
		contact->m_contactPoint = vertex;
		contact->setBodyData(one.m_body, two.m_body,
			data->m_friction, data->m_restitution);
		data->addContacts(1);
		return 1;
	}
	return 0;
}
#undef CHECK_OVERLAP




unsigned CollisionDetector::boxAndPoint(
	const ColliderBox &box,
	const Vector3 &point,
	CollisionData *data
	)
{
	// Transform the point into box coordinates
	Vector3 relPt = box.m_body->m_parent->getTransform()->getTransformation().transformInverse(point);

	Vector3 normal;

	// Check each axis, looking for the axis on which the
	// penetration is least deep.
	float min_depth = box.m_halfSize[0] - fabsf(relPt[0]);
	if(min_depth < 0) return 0;
	normal = box.getAxis(0) * ((relPt[0] < 0) ? -1 : 1);

	float depth = box.m_halfSize[1] - fabsf(relPt[1]);
	if(depth < 0) return 0;
	else if(depth < min_depth)
	{
		min_depth = depth;
		normal = box.getAxis(1) * ((relPt[1] < 0) ? -1 : 1);
	}

	depth = box.m_halfSize[2] - fabsf(relPt[2]);
	if(depth < 0) return 0;
	else if(depth < min_depth)
	{
		min_depth = depth;
		normal = box.getAxis(2) * ((relPt[2] < 0) ? -1 : 1);
	}

	// Compile the contact
	Contact* contact = data->m_contacts;
	contact->m_contactNormal = normal;
	contact->m_contactPoint = point;
	contact->m_penetration = min_depth;

	// Note that we don't know what rigid body the point
	// belongs to, so we just use NULL. Where this is called
	// this value can be left, or filled in.
	contact->setBodyData(box.m_body, NULL,
		data->m_friction, data->m_restitution);

	data->addContacts(1);
	return 1;
}

unsigned CollisionDetector::boxAndSphere(
	const ColliderBox &box,
	const ColliderSphere &sphere,
	CollisionData *data
	)
{
	// Transform the centre of the sphere into box coordinates
	Vector3 centre = sphere.getAxis(3);
	Vector3 relCentre = box.m_body->m_parent->getTransform()->getTransformation().transformInverse(centre);

	// Early out check to see if we can exclude the contact
	if(fabsf(relCentre[0]) - sphere.m_radius > box.m_halfSize[0] ||
	   fabsf(relCentre[1]) - sphere.m_radius > box.m_halfSize[1] ||
	   fabsf(relCentre[2]) - sphere.m_radius > box.m_halfSize[2])
	{
		return 0;
	}

	Vector3 closestPt(0, 0, 0);
	float dist;

	// Clamp each coordinate to the box.
	dist = relCentre[0];
	if(dist > box.m_halfSize[0]) dist = box.m_halfSize[0];
	if(dist < -box.m_halfSize[0]) dist = -box.m_halfSize[0];
	closestPt[0] = dist;

	dist = relCentre[1];
	if(dist > box.m_halfSize[1]) dist = box.m_halfSize[1];
	if(dist < -box.m_halfSize[1]) dist = -box.m_halfSize[1];
	closestPt[1] = dist;

	dist = relCentre[2];
	if(dist > box.m_halfSize[2]) dist = box.m_halfSize[2];
	if(dist < -box.m_halfSize[2]) dist = -box.m_halfSize[2];
	closestPt[2] = dist;

	// Check we're in contact
	dist = (closestPt - relCentre).squareLength();
	if(dist > sphere.m_radius * sphere.m_radius) return 0;

	// Compile the contact
	Vector3 closestPtWorld = box.m_body->m_parent->getTransform()->getTransformation().transform(closestPt);

	Contact* contact = data->m_contacts;
	contact->m_contactNormal = (closestPtWorld - centre);
	contact->m_contactNormal = contact->m_contactNormal.normalised();
	contact->m_contactPoint = closestPtWorld;
	contact->m_penetration = sphere.m_radius - sqrtf(dist);
	contact->setBodyData(box.m_body, sphere.m_body,
		data->m_friction, data->m_restitution);

	data->addContacts(1);
	return 1;
}

unsigned CollisionDetector::boxAndHalfSpace(
	const ColliderBox &box,
	const ColliderPlane &plane,
	CollisionData *data
	)
{
	// Make sure we have contacts
	if(data->m_contactsLeft <= 0) return 0;

	// Check for intersection
	//if(!IntersectionTests::boxAndHalfSpace(box, plane))
	//{
	//	return 0;
	//}

	// We have an intersection, so find the intersection points. We can make
	// do with only checking vertices. If the box is resting on a plane
	// or on an edge, it will be reported as four or two contact points.

	// Go through each combination of + and - for each half-size
	static float mults[8][3] = { { 1, 1, 1 }, { -1, 1, 1 }, { 1, -1, 1 }, { -1, -1, 1 },
								 { 1, 1, -1 }, { -1, 1, -1 }, { 1, -1, -1 }, { -1, -1, -1 } };

	Contact* contact = data->m_contacts;
	unsigned contactsUsed = 0;
	for(unsigned i = 0; i < 8; i++) {

		// Calculate the position of each vertex
		Vector3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);
		vertexPos *= box.m_halfSize;
		//vertexPos = box.m_body->m_parent->getTransform()->getTransformation().transform(vertexPos);
		vertexPos += box.m_body->getPosition();

		// Calculate the distance from the plane
		float vertexDistance = vertexPos.scalarProduct(plane.m_normal);

		// Compare this to the plane's distance
		if(vertexDistance <= plane.m_distance)
		{
			// Create the contact data.

			// The contact point is halfway between the vertex and the
			// plane - we multiply the direction by half the separation
			// distance and add the vertex location.
			contact->m_contactPoint = plane.m_normal;
			contact->m_contactPoint *= (vertexDistance - plane.m_distance);
			contact->m_contactPoint += vertexPos;
			contact->m_contactNormal = plane.m_normal;
			contact->m_penetration = plane.m_distance - vertexDistance;

			// Write the appropriate data
			contact->setBodyData(box.m_body, NULL,
				data->m_friction, data->m_restitution);

			// Move onto the next contact
			contact++;
			contactsUsed++;
			if(contactsUsed == (unsigned)data->m_contactsLeft) return contactsUsed;
		}
	}

	data->addContacts(contactsUsed);
	return contactsUsed;
}