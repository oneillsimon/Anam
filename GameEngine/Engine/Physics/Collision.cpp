#include <assert.h>

#include "Collision.h"

static bool tryAxis(const ColliderBox& one, const ColliderBox& two, Vector3 axis, const Vector3& toCentre, unsigned index, float& smallestPenetration, unsigned &smallestCase);
static float penetrationOnAxis(const ColliderBox& one, const ColliderBox& two, const Vector3 &axis, const Vector3 &toCentre);
void fillPointFaceBoxBox(const ColliderBox& one, const ColliderBox& two, const Vector3 &toCentre, CollisionData_ *data, unsigned best, float pen);
static Vector3 contactPoint(const Vector3 &pOne, const Vector3 &dOne, float oneSize, const Vector3 &pTwo, const Vector3 &dTwo, float twoSize, bool useOne);
static float transformToAxis(const ColliderBox& box, const Vector3& axis);

bool CollisionTester::sphereAndSphere(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data)
{
	ColliderSphere& s0 = *(ColliderSphere*)p0.getCollider();
	ColliderSphere& s1 = *(ColliderSphere*)p1.getCollider();

	Vector3 normal = p0.getTransform()->getPosition() - p1.getTransform()->getPosition();
	float distSq = normal.dot(normal);
	float radii = s0.m_radius + s1.m_radius;

	if(distSq < radii * radii)
	{
		if(data)
		{
			data->m_penetration = radii - sqrtf(distSq);
			normal = normal.normalised();
			data->m_normal = normal;
			data->m_point = p0.getTransform()->getPosition() - normal * (s0.m_radius - data->m_penetration * 0.5f);
		}
		return true;
	}
	return false;
}

bool CollisionTester::planeAndSphere(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data)
{
	ColliderPlane& plane = *(ColliderPlane*)p0.getCollider();
	ColliderSphere& sphere = *(ColliderSphere*)p1.getCollider();

	float seperation = p1.getTransform()->getPosition().dot(plane.m_normal - plane.m_distance);

	if(seperation > sphere.m_radius)
	{
		return false;
	}

	if(data)
	{
		data->m_penetration = sphere.m_radius - seperation;
		data->m_normal = plane.m_normal * -1.0f;
		data->m_point = p1.getTransform()->getPosition() - plane.m_normal * seperation;
	}
	
	return true;
}

#define CHECK_OVERLAP(axis, index) \
    if (!tryAxis(one, two, (axis), toCentre, (index), pen, best)) return 0;

bool CollisionTester::boxAndBox(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data)
{
	ColliderBox& one = *(ColliderBox*)p0.getCollider();
	ColliderBox& two = *(ColliderBox*)p1.getCollider();

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

		return true;
	}
	else if(best < 6)
	{
		fillPointFaceBoxBox(two, one, toCentre * -1.0f, data, best - 3, pen);

		return true;
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

		ptOnOneEdge = one.m_parent->getTransform()->getTransformation() * ptOnOneEdge;
		ptOnTwoEdge = two.m_parent->getTransform()->getTransformation() * ptOnTwoEdge;

		Vector3 vertex = contactPoint(ptOnOneEdge,
			oneAxis,
			one.m_halfSize[oneAxisIndex],
			ptOnTwoEdge,
			twoAxis,
			two.m_halfSize[twoAxisIndex],
			bestSingleAxis > 2);

		data->m_penetration = pen;
		data->m_normal = axis;
		data->m_point = vertex;

		return true;
	}

	return false;
}
#undef CHECK_OVERLAP

float getProjectedRadius(const ColliderOBB& o, const Vector3& d)
{
	Vector3 ox = o.m_parent->getTransform()->getRotation().getRight();
	Vector3 oy = o.m_parent->getTransform()->getRotation().getUp();
	Vector3 oz = o.m_parent->getTransform()->getRotation().getForward();

	return o.e[0] * fabsf(d.dot(ox)) +
		   o.e[1] * fabsf(d.dot(oy)) +
		   o.e[2] * fabsf(d.dot(oz));
}

bool seperateOnAxis(const ColliderOBB& a, const ColliderOBB& b, const Vector3& d)
{
	Vector3 dist = b.m_parent->getTransform()->getPosition() - a.m_parent->getTransform()->getPosition();
	float r = fabsf(dist.dot(d));
	float t = getProjectedRadius(a, d) + getProjectedRadius(b, d);

	return t < r;
}

bool CollisionTester::oBBAndOBB(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data)
{
	float ra, rb;
	Matrix4 R, AbsR;
	float len = 0.0f;

	ColliderOBB& a = *(ColliderOBB*)p0.getCollider();
	ColliderOBB& b = *(ColliderOBB*)p1.getCollider();

	Vector3 au[3] = { p0.getTransform()->getRotation().getRight(),
					  p0.getTransform()->getRotation().getUp(),
					  p0.getTransform()->getRotation().getForward() };

	Vector3 bu[3] = { p1.getTransform()->getRotation().getRight(),
					  p1.getTransform()->getRotation().getUp(),
					  p1.getTransform()->getRotation().getForward() };

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			R[i][j] = au[i].dot(bu[j]);
		}
	}

	Vector3 dir = b.m_parent->getTransform()->getPosition() - a.m_parent->getTransform()->getPosition();
	Vector3 t = Vector3(dir.dot(au[0]), dir.dot(au[1]), dir.dot(au[2]));

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			AbsR[i][j] = abs(R[i][j]) + FLT_EPSILON;
		}
	}

	for(int i = 0; i < 3; i++)
	{
		float ra = a.e[i];
		float rb = b.e[0] * AbsR[i][0] + b.e[1] * AbsR[i][1] + b.e[2] * AbsR[i][2];
		
		if(abs(t[i]) > ra + rb)
		{
			return false;
		}
	}

	for(int i = 0; i < 3; i++)
	{
		ra = a.e[0] * AbsR[0][i] + a.e[1] * AbsR[1][i] + a.e[2] * AbsR[2][i];
		rb = b.e[i];

		if(abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb)
		{
			return false;
		}
	}

	// Test axis L + A0 x B0
	ra = a.e[1] * AbsR[2][0] + a.e[2] * AbsR[1][0];
	rb = b.e[1] * AbsR[0][2] + b.e[2] * AbsR[0][1];

	if(abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb)
	{
		return false;
	}

	// Test axis L = A0 x B1
	ra = a.e[1] * AbsR[2][1] + a.e[2] * AbsR[1][1];
	rb = b.e[0] * AbsR[0][2] + b.e[2] * AbsR[0][0];

	if(abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb)
	{
		return false;
	}

	// Test axis L = A0 x B2
	ra = a.e[1] * AbsR[2][2] + a.e[2] * AbsR[1][2];
	rb = b.e[0] * AbsR[0][1] + b.e[1] * AbsR[0][0];

	if(abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb)
	{
		return false;
	}

	// Test axis L = A1 x B0
	ra = a.e[0] * AbsR[2][0] + a.e[2] * AbsR[0][0];
	rb = b.e[1] * AbsR[1][2] + b.e[2] * AbsR[1][1];

	if(abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)
	{
		return false;
	}

	// Test axis L = A1 x B1
	ra = a.e[0] * AbsR[2][1] + a.e[2] * AbsR[0][1];
	rb = b.e[0] * AbsR[1][2] + b.e[2] * AbsR[1][0];

	if(abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb)
	{
		return false;
	}

	// Test axis L = A1 x B2
	ra = a.e[0] * AbsR[2][2] + a.e[2] * AbsR[0][2];
	rb = b.e[0] * AbsR[1][1] + b.e[1] * AbsR[1][0];

	if(abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)
	{
		return false;
	}

	// Test axis L = A2 x B0
	ra = a.e[0] * AbsR[1][0] + a.e[1] * AbsR[0][0];
	rb = b.e[1] * AbsR[2][2] + b.e[2] * AbsR[2][1];

	if(abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb)
	{
		return false;
	}

	// Test axis L = A2 x B1
	ra = a.e[0] * AbsR[1][1] + a.e[1] * AbsR[0][1];
	rb = b.e[0] * AbsR[2][2] + b.e[2] * AbsR[2][0];

	if(abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb)
	{
		return false;
	}

	// Test axis L = A2 x B2
	ra = a.e[0] * AbsR[1][2] + a.e[1] * AbsR[0][2];
	rb = b.e[0] * AbsR[2][1] + b.e[1] * AbsR[2][0];

	if(abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb)
	{
		return false;
	}

	float f = abs(t[1] * R[0][2] - t[0] * R[1][2]);
	float f1 = ra + rb;

	Vector3 p = b.m_parent->getTransform()->getTransformedPoint(t);
	Vector3 n = b.m_parent->getTransform()->getPosition() - p;
	n = n.normalised();
	data->m_normal = n;
	data->m_point = t;
	data->m_penetration = f1;

	printf("intersecting\n");
	return true;
}

void CollisionTester::addCollisionImpulse(PhysicsObject& p0, PhysicsObject& p1, CollisionData_& data)
{
	if(p0.getInverseMass() + p1.getInverseMass() == 0.0f)
	{
		return;
	}

	Vector3 r0 = data.m_point - p0.getTransform()->getPosition();
	Vector3 r1 = data.m_point - p1.getTransform()->getPosition();

	Vector3 v0 = p0.getLinearVelocity() + p0.getAngularVelocity().cross(r0);
	Vector3 v1 = p1.getLinearVelocity() + p1.getAngularVelocity().cross(r1);

	Vector3 dv = v0 - v1;
	float relMov = -dv.dot(data.m_normal);

	if(relMov < -0.01f)
	{
		return;
	}

	//float e = 0.0f;
	float e = 1.0f;//random(0, 5);
	float normDiv = (p0.getInverseMass() + p1.getInverseMass()) +
					 data.m_normal.dot(p0.getInverseInertiaTensor() * r0.cross(data.m_normal).cross(r0) +
				     p1.getInverseInertiaTensor() * r1.cross(data.m_normal).cross(r1));

	float jn = -1 * (1 + e) * dv.dot(data.m_normal) / normDiv;
	jn = jn + (data.m_penetration * 0.01f);

	Vector3 l0 = p0.getLinearVelocity() + data.m_normal * (jn * p0.getInverseMass());
	Vector3 l1 = p1.getLinearVelocity() - data.m_normal * (jn * p1.getInverseMass());
	p0.setLinearVelocity(l0);
	p1.setLinearVelocity(l1);
	
	Vector3 a0 = p0.getAngularVelocity() + p0.getInverseInertiaTensor() * r0.cross(data.m_normal * jn);
	Vector3 a1 = p1.getAngularVelocity() - p1.getInverseInertiaTensor() * r1.cross(data.m_normal * jn);
	p0.setAngularVelocity(a0);
	p1.setAngularVelocity(a1);

	///
	
	Vector3 tangent = dv - data.m_normal * dv.dot(data.m_normal);
	tangent = tangent.normalised();

	float tangDiv = (p0.getInverseMass() + p1.getInverseMass()) +
		tangent.dot((p0.getInverseInertiaTensor() * r0.cross(tangent)).cross(r0) +
					(p1.getInverseInertiaTensor() * r1.cross(tangent)).cross(r1));
	float jt = -1 * dv.dot(tangent) / tangDiv;

	Vector3 l0_ = p0.getLinearVelocity() + tangent * (jt * p0.getInverseMass());
	Vector3 l1_ = p1.getLinearVelocity() - tangent * (jt * p1.getInverseMass());
	p0.setLinearVelocity(l0_);
	p1.setLinearVelocity(l1_);

	Vector3 a0_ = p0.getAngularVelocity() + p0.getInverseInertiaTensor() * r0.cross(tangent * jt);
	Vector3 a1_ = p1.getAngularVelocity() - p1.getInverseInertiaTensor() * r1.cross(tangent * jt);
	p0.setAngularVelocity(a0_);
	p1.setAngularVelocity(a1_);
}

static bool tryAxis(const ColliderBox& one, const ColliderBox& two, Vector3 axis, const Vector3& toCentre, unsigned index, float& smallestPenetration, unsigned &smallestCase)
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

static float penetrationOnAxis(const ColliderBox& one, const ColliderBox& two, const Vector3 &axis, const Vector3 &toCentre)
{
	float oneProject = transformToAxis(one, axis);
	float twoProject = transformToAxis(two, axis);

	float distance = fabsf(toCentre.scalarProduct(axis));

	return oneProject + twoProject - distance;
}

void fillPointFaceBoxBox(const ColliderBox& one, const ColliderBox& two, const Vector3 &toCentre, CollisionData_ *data, unsigned best, float pen)
{
	Vector3 normal = one.getAxis(best);

	if(one.getAxis(best).scalarProduct(toCentre) > 0)
	{
		normal = normal * -1.0f;
	}

	Vector3 vertex = two.m_halfSize;

	if(two.getAxis(0).scalarProduct(normal) < 0)
	{
		vertex.setX(-vertex.getX());
	}

	if(two.getAxis(1).scalarProduct(normal) < 0)
	{
		vertex.setY(-vertex.getY());
	}

	if(two.getAxis(2).scalarProduct(normal) < 0)
	{
		vertex.setZ(-vertex.getZ());
	}

	data->m_normal = normal;
	data->m_penetration = pen;
	data->m_point = two.m_parent->getTransform()->getTransformation() * normal;
}

static Vector3 contactPoint(const Vector3 &pOne, const Vector3 &dOne, float oneSize, const Vector3 &pTwo, const Vector3 &dTwo, float twoSize, bool useOne)
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

static float transformToAxis(const ColliderBox& box, const Vector3& axis)
{
	return box.m_halfSize.getX() * fabsf(axis.scalarProduct(box.getAxis(0))) +
		   box.m_halfSize.getY() * fabsf(axis.scalarProduct(box.getAxis(1))) +
		   box.m_halfSize.getZ() * fabsf(axis.scalarProduct(box.getAxis(2)));
}