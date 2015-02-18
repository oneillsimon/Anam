#include <assert.h>

#include "Contacts.h"

void Contact::setBodyData(RigidBody* one, RigidBody* two, float friction, float restitution)
{
	m_body[0] = one;
	m_body[1] = two;
	m_friction = friction;
	m_restitution = restitution;
}

void Contact::matchAwakeState()
{
	if(!m_body[1])
	{
		return;
	}

	bool body0awake = m_body[0]->getAwake();
	bool body1awake = m_body[1]->getAwake();

	if(body0awake ^ body1awake)
	{
		if(body0awake)
		{
			m_body[1]->setAwake();
		}
		else
		{
			m_body[0]->setAwake();
		}
	}
}

void Contact::swapBodies()
{
	m_contactNormal *= -1;
	RigidBody* temp = m_body[0];
	m_body[0] = m_body[1];
	m_body[1] = temp;
}

void Contact::calculateContactBasis()
{
	Vector3 contactTangent[2];

	if(fabsf(m_contactNormal.getX()) > fabsf(m_contactNormal.getY()))
	{
		const float s = 1.0f / sqrtf(m_contactNormal.getZ() * m_contactNormal.getZ() +
			m_contactNormal.getX() * m_contactNormal.getX());

		contactTangent[0].setX(m_contactNormal.getZ() * s);
		contactTangent[0].setY(0);
		contactTangent[0].setZ(-m_contactNormal.getX() * s);

		contactTangent[1].setX(m_contactNormal.getY() * contactTangent[0].getX());
		contactTangent[1].setY(m_contactNormal.getZ() * contactTangent[0].getX() -
			m_contactNormal.getX() * contactTangent[0].getZ());
		contactTangent[1].setZ(-m_contactNormal.getY() * contactTangent[0].getX());
	}
	else
	{
		const float s = 1.0f / sqrtf(m_contactNormal.getZ() * m_contactNormal.getZ() +
			m_contactNormal.getY() * m_contactNormal.getY());

		contactTangent[0].setX(0);
		contactTangent[0].setY(-m_contactNormal.getZ() * s);
		contactTangent[0].setZ(m_contactNormal.getY() * s);

		contactTangent[1].setX(m_contactNormal.getY() * contactTangent[0].getZ() -
							   m_contactNormal.getZ() * contactTangent[0].getY());
		contactTangent[1].setY(-m_contactNormal.getX() * contactTangent[0].getZ());
		contactTangent[1].setZ(m_contactNormal.getX() * contactTangent[0].getY());
	}

	m_contactToWorld.setComponents(m_contactNormal, contactTangent[0], contactTangent[1]);
}

Vector3 Contact::calculateLocalVelocity(unsigned bodyIndex, float duration)
{
	RigidBody* thisBody = m_body[bodyIndex];

	Vector3 velocity = thisBody->getRotation() % m_relativeContactPosition[bodyIndex];
	velocity += thisBody->getVelocity();

	Vector3 contactVelocity = m_contactToWorld.transformTranspose(velocity);
	Vector3 accVelocity = thisBody->getLastFrameAcceleration() * duration;

	accVelocity = m_contactToWorld.transformTranspose(accVelocity);
	accVelocity.setX(0);
	contactVelocity += accVelocity;
	return contactVelocity;
}

void Contact::calculateDesiredDeltaVelocity(float duration, const Vector3& contactVelocity)
{
	const static float velocityLimit = 5.0f;

	float velocityFromAcc = 0;

	if(m_body[0]->getAwake())
	{
		velocityFromAcc += m_body[0]->getLastFrameAcceleration().scalarProduct(m_contactNormal) * duration;
	}

	if(m_body[1] && m_body[1]->getAwake())
	{
		velocityFromAcc -= m_body[1]->getLastFrameAcceleration().scalarProduct(m_contactNormal) * duration;
	}

	float thisRestitution = m_restitution;

	if(fabsf(m_contactVelocity.getX()) < velocityLimit)
	{
		thisRestitution = 0.0f;
	}

	m_desiredDeltaVelocity = -contactVelocity.getX() - thisRestitution * (contactVelocity.getX() - velocityFromAcc);
}

void Contact::calculateInternals(float duration)
{
	if(!m_body[0])
	{
		swapBodies();
	}

	assert(m_body[0]);

	calculateContactBasis();

	m_relativeContactPosition[0] = m_contactPoint - m_body[0]->getPosition();
	m_relativeContactPosition[0] = m_relativeContactPosition[0].normalised() * 2.0f;

	if(m_body[1])
	{
		m_relativeContactPosition[1] = m_contactPoint - m_body[1]->getPosition();
		m_relativeContactPosition[1] = m_relativeContactPosition[1].normalised() * 2.0f;
	}

	m_contactVelocity = Vector3();

	m_contactVelocity = calculateLocalVelocity(0, duration);

	if(m_body[1])
	{
		m_contactVelocity -= calculateLocalVelocity(1, duration);
	}

	calculateDesiredDeltaVelocity(duration, m_contactVelocity);
}

void Contact::applyVelocityChange(Vector3 velocityChange[2])
{
	Matrix3 inverseInertiaTensor[2];

	inverseInertiaTensor[0] = m_body[0]->getInverseInertiaTensorWorld();

	if(m_body[1])
	{
		inverseInertiaTensor[1] = m_body[1]->getInverseInertiaTensorWorld();
	}

	Vector3 impulseContact;

	if(m_friction == 0.0f)
	{
		impulseContact = calculateFrictionlessImpulse(inverseInertiaTensor);
	}
	else
	{
		impulseContact = calculateFrictionImpulse(inverseInertiaTensor);
	}

	Vector3 impulse = m_contactToWorld.transform(impulseContact) * m_contactNormal;
	velocityChange[0] = Vector3();
	velocityChange[0] += impulse * m_body[0]->getInverseMass();

	m_body[0]->addVelocity(velocityChange[0]);

	if(m_body[1])
	{
		velocityChange[1] = Vector3();
		velocityChange[1] += impulse * m_body[1]->getInverseMass() * -1.0f;

		m_body[1]->addVelocity(velocityChange[1]);
	}
}

Vector3 Contact::calculateFrictionlessImpulse(Matrix3* inverseInertiaTensor)
{
	Vector3 impulseContact;

	Vector3 deltaVelocityWorld = m_relativeContactPosition[0] % m_contactNormal;
	deltaVelocityWorld = inverseInertiaTensor[0].transform(deltaVelocityWorld);
	deltaVelocityWorld = deltaVelocityWorld % m_relativeContactPosition[0];

	float deltaVelocity = deltaVelocityWorld.scalarProduct(m_contactNormal);
	deltaVelocity += m_body[0]->getInverseMass();

	if(m_body[1])
	{
		Vector3 deltaVelocityWorld = m_relativeContactPosition[1] % m_contactNormal;
		deltaVelocityWorld = inverseInertiaTensor[1].transform(deltaVelocityWorld);
		deltaVelocityWorld = deltaVelocityWorld % m_relativeContactPosition[1];

		deltaVelocity = deltaVelocityWorld.scalarProduct(m_contactNormal);
		deltaVelocity += m_body[1]->getInverseMass();
	}

	impulseContact.setX(m_desiredDeltaVelocity / deltaVelocity);
	impulseContact.setY(0);
	impulseContact.setZ(0);

	return impulseContact;
}

Vector3 Contact::calculateFrictionImpulse(Matrix3* inverseIntertiaTensor)
{
	Vector3 impulseContact;
	float inverseMass = m_body[0]->getInverseMass();

	Matrix3 impulseToTorque;
	impulseToTorque.setSkewSymmetric(m_relativeContactPosition[0]);

	Matrix3 deltaVelocityWorld = impulseToTorque;
	deltaVelocityWorld *= inverseIntertiaTensor[0];
	deltaVelocityWorld *= impulseToTorque;
	deltaVelocityWorld *= -1;

	if(m_body[1])
	{
		impulseToTorque.setSkewSymmetric(m_relativeContactPosition[1]);

		Matrix3 deltaVelocityWorld2 = impulseToTorque;
		deltaVelocityWorld2 *= inverseIntertiaTensor[1];
		deltaVelocityWorld2 *= impulseToTorque;
		deltaVelocityWorld2 *= -1;

		deltaVelocityWorld += deltaVelocityWorld2;
		inverseMass += m_body[1]->getInverseMass();
	}

	Matrix3 deltaVelocity = m_contactToWorld.transpose();
	deltaVelocity *= deltaVelocityWorld;
	deltaVelocity *= m_contactToWorld;

	deltaVelocity.setAt(0, 0, deltaVelocity.getAt(0, 0) + inverseMass);
	deltaVelocity.setAt(1, 1, deltaVelocity.getAt(1, 1) + inverseMass);
	deltaVelocity.setAt(2, 2, deltaVelocity.getAt(2, 2) + inverseMass);

	Matrix3 impulseMatrix = deltaVelocity.inverse();

	Vector3 velKill(m_desiredDeltaVelocity, -m_contactVelocity.getY(), -m_contactVelocity.getZ());

	impulseContact = impulseMatrix.transform(velKill);

	float planarImpulse = sqrtf(impulseContact.getY() * impulseContact.getY() +
								impulseContact.getZ() * impulseContact.getZ());

	if(planarImpulse > impulseContact.getX() * m_friction)
	{
		impulseContact.setY(impulseContact.getY() / planarImpulse);
		impulseContact.setZ(impulseContact.getZ() / planarImpulse);
		impulseContact.setX(deltaVelocity.getAt(0, 0) +
							deltaVelocity.getAt(1, 0) * m_friction * impulseContact.getY() +
							deltaVelocity.getAt(2, 0) * m_friction * impulseContact.getZ());

		impulseContact.setX(m_desiredDeltaVelocity / impulseContact.getX());
		impulseContact.setY(impulseContact.getY() * m_friction * impulseContact.getX());
		impulseContact.setZ(impulseContact.getZ() * m_friction * impulseContact.getX());
	}

	return impulseContact;
}

void Contact::applyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], float penetration)
{
	const float angularLimit = 0.2f;
	float angularMove[2];
	float linearMove[2];

	float totalInertia = 0;
	float linearInertia[2];
	float angularInertia[2];

	for(unsigned i = 0; i < 2; i++)
	{
		if(m_body[i])
		{
			Matrix3 inverseIntertiaTensor = m_body[i]->getInverseInertiaTensorWorld();

			Vector3 angularIntertiaWorld = m_relativeContactPosition[i] % m_contactNormal;
			angularIntertiaWorld = inverseIntertiaTensor.transform(angularIntertiaWorld);
			angularIntertiaWorld = angularIntertiaWorld % m_relativeContactPosition[i];
			angularInertia[i] = angularIntertiaWorld.scalarProduct(m_contactNormal);

			linearInertia[i] = m_body[i]->getInverseMass();
			totalInertia += linearInertia[i] + angularInertia[i];
		}
	}

	for(unsigned i = 0; i < 2; i++)
	{
		if(m_body[i])
		{
			float sign = (i == 0) ? 1 : -1;
			angularMove[i] = sign * penetration * (angularInertia[i] / totalInertia);
			linearMove[i] = sign * penetration * (linearInertia[i] / totalInertia);

			Vector3 projection = m_relativeContactPosition[i];
			projection += (m_contactNormal * -m_relativeContactPosition[i].scalarProduct(m_contactNormal));

			float maxMagnitude = angularLimit * projection.length();

			if(angularMove[i] <= -maxMagnitude)
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
				Vector3 targetAngularDirection = m_relativeContactPosition[i] * m_contactNormal;

				Matrix3 inverseIntertiaTensor = m_body[i]->getInverseInertiaTensorWorld();

				angularChange[i] = inverseIntertiaTensor.transform(targetAngularDirection) *
					(angularMove[i] / angularInertia[i]);
			}

			linearChange[i] = m_contactNormal * linearMove[i];

			Vector3 pos = m_body[i]->getPosition();
			pos += (m_contactNormal * linearMove[i]);
			m_body[i]->setPosition(pos);

			Quaternion q = m_body[i]->getOrientation();
			q = q + (angularChange[i] * 1.0f);
			m_body[i]->setOrientation(q);

			if(!m_body[i]->getAwake())
			{
				m_body[i]->calculateDerivedData();
			}
		}
	}
}

ContactResolver::ContactResolver(unsigned iterations, float velocityEpsilon, float positionEpsilon)
{
	setIterations(iterations, iterations);
	setEpsilon(velocityEpsilon, positionEpsilon);
}

ContactResolver::ContactResolver(unsigned velocityIterations, unsigned positionIterations, float velocityEpsilon, float positionEpsilon)
{
	setIterations(velocityIterations);
	setEpsilon(velocityEpsilon, positionEpsilon);
}

void ContactResolver::setIterations(unsigned iterations)
{
	setIterations(iterations, iterations);
}

void ContactResolver::setIterations(unsigned velocityIterations, unsigned positionIterations)
{
	m_velocityIterations = velocityIterations;
	m_positionIterations = positionIterations;
}

void ContactResolver::setEpsilon(float velocityEpsilon, float positionEpsilon)
{
	m_velocityEpsilon = velocityEpsilon;
	m_positionEpsilon = positionEpsilon;
}

void ContactResolver::resolveContacts(Contact* contacts, unsigned numContacts, float duration)
{
	if(numContacts == 0)
	{
		return;
	}

	if(!isValid())
	{
		return;
	}

	prepareContacts(contacts, numContacts, duration);
	adjustPositions(contacts, numContacts, duration);
	adjustVelocities(contacts, numContacts, duration);
}

void ContactResolver::prepareContacts(Contact* contacts, unsigned numContacts, float duration)
{
	Contact* lastContact = contacts + numContacts;

	for(Contact* contact = contacts; contact < lastContact; contact++)
	{
		contact->calculateInternals(duration);
	}
}

void ContactResolver::adjustVelocities(Contact* contact, unsigned numContacts, float duration)
{
	//unsigned i;
	unsigned index;
	Vector3 velocityChange[2];
	float max;
	Vector3 deltaVelocity;

	m_velocityIterationsUsed = 0;

	while(m_velocityIterationsUsed < m_velocityIterations)
	{
		max = m_velocityEpsilon;
		index = numContacts;

		for(unsigned i = 0; i < numContacts; i++)
		{
			if(contact[i].m_desiredDeltaVelocity > max)
			{
				max = contact[i].m_desiredDeltaVelocity;
				index = i;
			}
		}

		if(index == numContacts)
		{
			break;
		}

		contact[index].matchAwakeState();
		contact[index].applyVelocityChange(velocityChange);

		for(unsigned i = 0; i < numContacts; i++)
		{
			for(unsigned b = 0; b < 2; b++)
			{
				if(contact[i].m_body[b])
				{
					for(unsigned d = 0; d < 2; d++)
					{
						if(contact[i].m_body[b] == contact[index].m_body[d])
						{
							Vector3 vC = velocityChange[d];
							
							deltaVelocity = vC * contact[i].m_relativeContactPosition[b];
							contact[i].m_contactVelocity += contact[i].m_contactToWorld.transformTranspose(deltaVelocity) * (b ? -1 : 1);
							contact[i].calculateDesiredDeltaVelocity(duration, contact[i].m_contactVelocity);
							//contact[i].m_contactVelocity = contact[i].m_desiredDeltaVelocity;
							//printf("%f\n", contact[i].m_desiredDeltaVelocity);
						}
					}
				}
			}
		}
		m_velocityIterationsUsed++;
	}
}

void ContactResolver::adjustPositions(Contact* contact, unsigned numContacts, float duration)
{
	unsigned i;
	unsigned index;
	Vector3 linearChange[2];
	Vector3 angularChange[2];
	float max;
	Vector3 deltaPosition;

	m_positionIterationsUsed = 0;

	while(m_positionIterationsUsed < m_positionIterations)
	{
		max = m_positionEpsilon;
		index = numContacts;

		for(i = 0; i < numContacts; i++)
		{
			if(contact[i].m_penetration > max)
			{
				max = contact[i].m_penetration;
				index = i;
			}
		}

		if(index == numContacts)
		{
			break;
		}

		contact[index].matchAwakeState();
		contact[index].applyPositionChange(linearChange, angularChange, max);

		for(i = 0; i < numContacts; i++)
		{
			for(unsigned b = 0; b < 2; b++)
			{
				if(contact[i].m_body[b])
				{
					for(unsigned d = 0; d < 2; d++)
					{
						if(contact[i].m_body[b] == contact[index].m_body[d])
						{
							deltaPosition = linearChange[d] + (angularChange[d] * contact[i].m_relativeContactPosition[b]);
							contact[i].m_penetration += deltaPosition.scalarProduct(contact[i].m_contactNormal) * (b ? 1 : -1);
						}
					}
				}
			}
		}
		m_positionIterationsUsed++;
	}
}