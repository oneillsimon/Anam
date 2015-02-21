#include <assert.h>

#include "Contacts.h"

void Contact::setBodyData(RigidBody* one, RigidBody *two,
	float friction, float restitution)
{
	m_body[0] = one;
	m_body[1] = two;
	m_friction = friction;
	m_restitution = restitution;
}

void Contact::matchAwakeState()
{
	if(!m_body[1]) return;

	bool body0awake = m_body[0]->getAwake();
	bool body1awake = m_body[1]->getAwake();

	if(body0awake ^ body1awake) {
		if(body0awake) m_body[1]->setAwake();
		else m_body[0]->setAwake();
	}
}

void Contact::swapBodies()
{
	m_contactNormal *= -1;

	RigidBody *temp = m_body[0];
	m_body[0] = m_body[1];
	m_body[1] = temp;
}

void Contact::calculateContactBasis()
{
	Vector3 contactTangent[2];

	if(fabsf(m_contactNormal[0]) > fabsf(m_contactNormal[1]))
	{
		const float s = 1.0f / fabsf(m_contactNormal[2] * m_contactNormal[2] +
			m_contactNormal[0] * m_contactNormal[0]);

		contactTangent[0][0] = m_contactNormal[2] * s;
		contactTangent[0][1] = 0;
		contactTangent[0][2] = m_contactNormal[0] * s;

		contactTangent[1][0] = m_contactNormal[1] * contactTangent[0][0];
		contactTangent[1][1] = m_contactNormal[2] * contactTangent[0][0] -
			m_contactNormal[1] * contactTangent[0][2];
		contactTangent[1][2] = -m_contactNormal[1] * contactTangent[0][0];
	}
	else
	{
		const float s = 1.0f / fabsf(m_contactNormal[2] * m_contactNormal[2] +
			m_contactNormal[1] * m_contactNormal[1]);

		contactTangent[0][0] = 0;
		contactTangent[0][1] = -m_contactNormal[2] *s;
		contactTangent[0][2] = m_contactNormal[1] * s;

		contactTangent[1][0] = m_contactNormal[1] * contactTangent[0][2] -
			m_contactNormal[2] * contactTangent[0][1];
		contactTangent[1][1] = -m_contactNormal[0] * contactTangent[0][2];
		contactTangent[1][2] = m_contactNormal[0] * contactTangent[0][1];
	}

	m_contactToWorld.setComponents(
		m_contactNormal,
		contactTangent[0],
		contactTangent[1]);
}

Vector3 Contact::calculateLocalVelocity(unsigned bodyIndex, float duration)
{
	RigidBody *thisBody = m_body[bodyIndex];

	Vector3 velocity = thisBody->getRotation() % m_relativeContactPosition[bodyIndex];
	velocity += thisBody->getVelocity();

	Vector3 contactVelocity = m_contactToWorld.transformTranspose(velocity);
	
	Vector3 accVelocity = thisBody->getLastFrameAcceleration() * duration;

	accVelocity = m_contactToWorld.transformTranspose(accVelocity);

	accVelocity[0] = 0;

	contactVelocity += accVelocity;

	//contactVelocity[0] = 0;
	
	return contactVelocity;
}


void Contact::calculateDesiredDeltaVelocity(float duration)
{
	const static float velocityLimit = 0.25f;

	float velocityFromAcc = 0;

	if(m_body[0]->getAwake())
	{
		velocityFromAcc +=
			m_body[0]->getLastFrameAcceleration().scalarProduct(m_contactNormal) * duration;
	}

	if(m_body[1] && m_body[1]->getAwake())
	{
		velocityFromAcc -=
			m_body[1]->getLastFrameAcceleration().scalarProduct(m_contactNormal) * duration;
	}

	float thisRestitution = m_restitution;
	if(fabsf(m_contactVelocity[0]) < velocityLimit)
	{
		thisRestitution = 0.0f;
	}

	m_desiredDeltaVelocity = -m_contactVelocity[0] - thisRestitution * (m_contactVelocity[0] - velocityFromAcc);
	//m_desiredDeltaVelocity = -1.0f;
}


void Contact::calculateInternals(float duration)
{
	if(!m_body[0]) swapBodies();
	assert(m_body[0]);

	calculateContactBasis();

	m_relativeContactPosition[0] = m_contactPoint - m_body[0]->getPosition();
	if(m_body[1]) {
		m_relativeContactPosition[1] = m_contactPoint - m_body[1]->getPosition();
	}

	m_contactVelocity = calculateLocalVelocity(0, duration);
	if(m_body[1]) {
		m_contactVelocity -= calculateLocalVelocity(1, duration);
	}
	
	calculateDesiredDeltaVelocity(duration);
}

void Contact::applyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2])
{
	// Get hold of the inverse mass and inverse inertia tensor, both in
	// world coordinates.
	Matrix3 inverseInertiaTensor[2];
	inverseInertiaTensor[0] = m_body[0]->getInverseInertiaTensorWorld();
	if(m_body[1])
		inverseInertiaTensor[1] = m_body[1]->getInverseInertiaTensorWorld();

	// We will calculate the impulse for each contact axis
	Vector3 impulseContact;

	if(m_friction == 0.0f)
	{
		// Use the short format for frictionless contacts
		impulseContact = calculateFrictionlessImpulse(inverseInertiaTensor);
	}
	else
	{
		// Otherwise we may have impulses that aren't in the direction of the
		// contact, so we need the more complex version.
		//impulseContact = Vector3(3, 0, 0);//calculateFrictionImpulse(inverseInertiaTensor);
		impulseContact = calculateFrictionImpulse(inverseInertiaTensor);
	}

	// Convert impulse to world coordinates
	Vector3 impulse = m_contactToWorld.transform(impulseContact);

	// Split in the impulse into linear and rotational components
	Vector3 impulsiveTorque = m_relativeContactPosition[0] % impulse;
	rotationChange[0] = inverseInertiaTensor[0].transform(impulsiveTorque);
	velocityChange[0] = Vector3();
	velocityChange[0].addScaledVector(impulse, m_body[0]->getInverseMass());

	// Apply the changes
	//m_body[0]->addVelocity(velocityChange[0]);
	m_body[0]->addVelocity(velocityChange[0]);
	m_body[0]->addRotation(rotationChange[0]);

	if(m_body[1])
	{
		// Work out body one's linear and angular changes
		Vector3 impulsiveTorque = impulse % m_relativeContactPosition[1];
		rotationChange[1] = inverseInertiaTensor[1].transform(impulsiveTorque);
		velocityChange[1] = Vector3();
		velocityChange[1].addScaledVector(impulse, -m_body[1]->getInverseMass());

		// And apply them.
		m_body[1]->addVelocity(velocityChange[1]);
		m_body[1]->addRotation(rotationChange[1]);
	}
}

Vector3 Contact::calculateFrictionlessImpulse(Matrix3 * inverseInertiaTensor)
{
	Vector3 impulseContact;

	Vector3 deltaVelWorld = m_relativeContactPosition[0] % m_contactNormal;
	deltaVelWorld = inverseInertiaTensor[0].transform(deltaVelWorld);
	deltaVelWorld = deltaVelWorld % m_relativeContactPosition[0];

	float deltaVelocity = deltaVelWorld.scalarProduct(m_contactNormal);

	deltaVelocity += m_body[0]->getInverseMass();

	if(m_body[1])
	{
		Vector3 deltaVelWorld = m_relativeContactPosition[1] % m_contactNormal;
		deltaVelWorld = inverseInertiaTensor[1].transform(deltaVelWorld);
		deltaVelWorld = deltaVelWorld % m_relativeContactPosition[1];

		deltaVelocity += deltaVelWorld.scalarProduct(m_contactNormal);

		deltaVelocity += m_body[1]->getInverseMass();
	}

	impulseContact[0] = m_desiredDeltaVelocity / deltaVelocity;
	impulseContact[1] = 0;
	impulseContact[2] = 0;
	return impulseContact;
}

Vector3 Contact::calculateFrictionImpulse(Matrix3 * inverseInertiaTensor)
{
	Vector3 impulseContact;
	float inverseMass = m_body[0]->getInverseMass();

	Matrix3 impulseToTorque;
	impulseToTorque.setSkewSymmetric(m_relativeContactPosition[0]);

	Matrix3 deltaVelWorld = impulseToTorque;
	deltaVelWorld *= inverseInertiaTensor[0];
	deltaVelWorld *= impulseToTorque;
	deltaVelWorld *= -1;

	if(m_body[1])
	{
		impulseToTorque.setSkewSymmetric(m_relativeContactPosition[1]);

		Matrix3 deltaVelWorld2 = impulseToTorque;
		deltaVelWorld2 *= inverseInertiaTensor[1];
		deltaVelWorld2 *= impulseToTorque;
		deltaVelWorld2 *= -1;

		deltaVelWorld += deltaVelWorld2;

		inverseMass += m_body[1]->getInverseMass();
	}

	Matrix3 deltaVelocity = m_contactToWorld.transpose();
	deltaVelocity *= deltaVelWorld;
	deltaVelocity *= m_contactToWorld;

	deltaVelocity.setAt(0, 0, deltaVelocity.getAt(0, 0) + inverseMass);
	deltaVelocity.setAt(1, 1, deltaVelocity.getAt(1, 1) + inverseMass);
	deltaVelocity.setAt(2, 2, deltaVelocity.getAt(2, 2) + inverseMass);

	Matrix3 impulseMatrix = deltaVelocity.inverse();

	Vector3 velKill(m_desiredDeltaVelocity,
					-m_contactVelocity[1],
					-m_contactVelocity[2]);

	impulseContact = impulseMatrix.transform(velKill);

	float planarImpulse = fabsf(impulseContact[1] * impulseContact[1] + impulseContact[2] * impulseContact[2]);

	if(planarImpulse > impulseContact[0] * m_friction)
	{
		impulseContact[1] /= planarImpulse;
		impulseContact[2] /= planarImpulse;

		impulseContact[0] = deltaVelocity.getAt(0, 0) +
							deltaVelocity.getAt(1, 0) * m_friction * impulseContact[1] +
							deltaVelocity.getAt(2, 0) * m_friction * impulseContact[2];

		impulseContact[0] = m_desiredDeltaVelocity / impulseContact[0];
		impulseContact[1] *= m_friction * impulseContact[0];
		impulseContact[2] *= m_friction * impulseContact[0];
	}

	return impulseContact;
}

void Contact::applyPositionChange(Vector3 linearChange[2],
	Vector3 angularChange[2],
	float penetration)
{
	const float angularLimit = 0.2f;
	float angularMove[2];
	float linearMove[2];

	float totalInertia = 0;
	float linearInertia[2];
	float angularInertia[2];

	for(unsigned i = 0; i < 2; i++) if(m_body[i])
	{
		Matrix3 inverseInertiaTensor  = m_body[i]->getInverseInertiaTensorWorld();

		Vector3 angularInertiaWorld =
			m_relativeContactPosition[i] % m_contactNormal;
		angularInertiaWorld =
			inverseInertiaTensor.transform(angularInertiaWorld);
		angularInertiaWorld =
			angularInertiaWorld % m_relativeContactPosition[i];
		angularInertia[i] =
			angularInertiaWorld.scalarProduct(m_contactNormal);

		linearInertia[i] = m_body[i]->getInverseMass();

		totalInertia += linearInertia[i] + angularInertia[i];

	}

	for(unsigned i = 0; i < 2; i++) if(m_body[i])
	{
		float sign = (i == 0) ? 1 : -1;
		angularMove[i] =
			sign * penetration * (angularInertia[i] / totalInertia);
		linearMove[i] =
			sign * penetration * (linearInertia[i] / totalInertia);

		Vector3 projection = m_relativeContactPosition[i];
		projection.addScaledVector(
			m_contactNormal,
			-m_relativeContactPosition[i].scalarProduct(m_contactNormal)
			);

		float maxMagnitude = angularLimit * projection.length();

		if(angularMove[i] < -maxMagnitude)
		{
			float totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = -maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}
		else if(angularMove[i] > maxMagnitude)
		{
			float totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}

		if(angularMove[i] == 0)
		{
			angularChange[i] = Vector3();
		}
		else
		{
			Vector3 targetAngularDirection =
				m_relativeContactPosition[i] * (m_contactNormal);

			Matrix3 inverseInertiaTensor = m_body[i]->getInverseInertiaTensorWorld();

			angularChange[i] =
				inverseInertiaTensor.transform(targetAngularDirection) *
				(angularMove[i] / angularInertia[i]);
		}

		linearChange[i] = m_contactNormal * linearMove[i];

		Vector3 pos = m_body[i]->getPosition();
		pos.addScaledVector(m_contactNormal, linearMove[i]);
		m_body[i]->setPosition(pos);

		Quaternion q = m_body[i]->getOrientation();

		q = q + (angularChange[i] * 1.0f);

		m_body[i]->setOrientation(q);

		if(!m_body[i]->getAwake()) m_body[i]->calculateDerivedData();
	}
}

ContactResolver::ContactResolver(unsigned iterations,
	float velocityEpsilon,
	float positionEpsilon)
{
	setIterations(iterations, iterations);
	setEpsilon(velocityEpsilon, positionEpsilon);
}

ContactResolver::ContactResolver(unsigned velocityIterations,
	unsigned positionIterations,
	float velocityEpsilon,
	float positionEpsilon)
{
	setIterations(velocityIterations);
	setEpsilon(velocityEpsilon, positionEpsilon);
}

void ContactResolver::setIterations(unsigned iterations)
{
	setIterations(iterations, iterations);
}

void ContactResolver::setIterations(unsigned velocityIterations,
	unsigned positionIterations)
{
	m_velocityIterations = velocityIterations;
	m_positionIterations = positionIterations;
}

void ContactResolver::setEpsilon(float velocityEpsilon,
	float positionEpsilon)
{
	m_velocityEpsilon = velocityEpsilon;
	m_positionEpsilon = positionEpsilon;
}

void ContactResolver::resolveContacts(Contact *contacts,
	unsigned numContacts,
	float duration)
{
	if(numContacts == 0) return;
	if(!isValid()) return;

	prepareContacts(contacts, numContacts, duration);

	adjustPositions(contacts, numContacts, duration);

	adjustVelocities(contacts, numContacts, duration);
}

void ContactResolver::prepareContacts(Contact* contacts,
	unsigned numContacts,
	float duration)
{
	Contact* lastContact = contacts + numContacts;

	for(Contact* contact = contacts; contact < lastContact; contact++)
	{
		contact->calculateInternals(duration);
	}
}

void ContactResolver::adjustVelocities(Contact *c, unsigned numContacts, float duration)
{
	Vector3 velocityChange[2], rotationChange[2];
	Vector3 deltaVel;

	m_velocityIterationsUsed = 0;

	while(m_velocityIterationsUsed < m_velocityIterations)
	{
		float max = m_velocityEpsilon;
		unsigned index = numContacts;

		for(unsigned i = 0; i < numContacts; i++)
		{
			c[i].calculateDesiredDeltaVelocity(duration);
			if(c[i].m_desiredDeltaVelocity >= max)
			{
				max = c[i].m_desiredDeltaVelocity;
				index = i;
			}
		}

		if(index == numContacts)
		{
			break;
		}

		c[index].matchAwakeState();
		c[index].applyVelocityChange(velocityChange, rotationChange);

		for(unsigned i = 0; i < numContacts; i++)
		{
			for(unsigned b = 0; b < 2; b++) if(c[i].m_body[b])
			{
				for(unsigned d = 0; d < 2; d++)
				{
					if(c[i].m_body[b] == c[index].m_body[d])
					{
						deltaVel = velocityChange[d] + rotationChange[d].vectorProduct(c[i].m_relativeContactPosition[b]);

						c[i].m_contactVelocity += c[i].m_contactToWorld.transformTranspose(deltaVel) * (b ? -1 : 1);
						c[i].calculateInternals(duration);
						c[i].calculateDesiredDeltaVelocity(duration);
					}
				}
			}
		}
		//printf("velocity pass %d of %d\n", m_velocityIterationsUsed, m_velocityIterations);
		m_velocityIterationsUsed++;
	}
}

void ContactResolver::adjustPositions(Contact *c,
	unsigned numContacts,
	float duration)
{
	unsigned i, index;
	Vector3 linearChange[2], angularChange[2];
	float max;
	Vector3 deltaPosition;

	m_positionIterationsUsed = 0;
	while(m_positionIterationsUsed < m_positionIterations)
	{
		max = m_positionEpsilon;
		index = numContacts;
		for(i = 0; i<numContacts; i++)
		{
			if(c[i].m_penetration > max)
			{
				max = c[i].m_penetration;
				index = i;
			}
		}
		if(index == numContacts) break;

		c[index].matchAwakeState();

		c[index].applyPositionChange(
			linearChange,
			angularChange,
			max);

		for(i = 0; i < numContacts; i++)
		{
			for(unsigned b = 0; b < 2; b++) if(c[i].m_body[b])
			{
				for(unsigned d = 0; d < 2; d++)
				{
					if(c[i].m_body[b] == c[index].m_body[d])
					{
						deltaPosition = linearChange[d] +
							angularChange[d].vectorProduct(
							c[i].m_relativeContactPosition[b]);

						c[i].m_penetration +=
							deltaPosition.scalarProduct(c[i].m_contactNormal)
							* (b ? 1 : -1);
					}
				}
			}
		}
		//printf("position pass %d of %d\n", m_positionIterationsUsed, m_positionIterations);
		m_positionIterationsUsed++;
	}
}
