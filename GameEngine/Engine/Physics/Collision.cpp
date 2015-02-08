#include "Collision.h"

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

bool CollisionTester::boxAndBox(PhysicsObject& p0, PhysicsObject& p1, CollisionData_* data)
{
	ColliderBox& b0 = *(ColliderBox*)p0.getCollider();
	ColliderBox& b1 = *(ColliderBox*)p1.getCollider();

	float dist = abs(p0.getTransform()->getPosition().getX() - p1.getTransform()->getPosition().getX());
	float sum = b0.m_halfSize.getX() + b1.m_halfSize.getX();
	float smallDist = 0;

	if(dist <= sum)
	{
		dist = abs(p0.getTransform()->getPosition().getY() - p1.getTransform()->getPosition().getY());
		sum = b0.m_halfSize.getY() + b1.m_halfSize.getY();
		smallDist = dist;

		if(dist <= sum)
		{
			dist = abs(p0.getTransform()->getPosition().getZ() - p1.getTransform()->getPosition().getZ());
			sum = b0.m_halfSize.getZ() + b1.m_halfSize.getZ();
			smallDist = fminf(dist, smallDist);

			if(dist <= sum)
			{
				Vector3 normal = p0.getTransform()->getPosition() - p1.getTransform()->getPosition();
				smallDist = fminf(dist, smallDist);

				data->m_normal = normal.normalised();
				data->m_penetration = smallDist;
				data->m_point = normal * b0.m_halfSize * 0.5f;//.length();// +p0.getTransform()->getPosition();

				return true;
			}
		}
	}
	return false;
}

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
	float e = 0.0f;//random(0, 5);
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