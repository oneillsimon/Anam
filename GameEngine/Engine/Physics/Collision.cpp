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

	// restituition??
	float e = 0.0f;
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