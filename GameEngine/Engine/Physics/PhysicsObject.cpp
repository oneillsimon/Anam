#include <iostream>

#include "PhysicsObject.h"

//PhysicsObject::PhysicsObject(Collider* collider, const Vector3& velocity) :
//	m_position(collider->getCentre()),
//	m_velocity(velocity),
//	m_collider(collider),
//	m_oldPosition(collider->getCentre())
//{
//}
//
//PhysicsObject::PhysicsObject(const PhysicsObject& other) :
//	m_position(other.m_position),
//	m_velocity(other.m_velocity),
//	m_collider(other.m_collider),
//	m_oldPosition(other.m_oldPosition)
//{
//	m_collider->addReference();
//}

PhysicsObject::PhysicsObject(const Vector3& position, const Quaternion& rotation, float scale)
	: GameObject(position, rotation, scale)
{

}

PhysicsObject::~PhysicsObject()
{
	if(m_collider->removeReference())
	{
		delete m_collider;
	}
}

void PhysicsObject::integrate(float delta)
{
	m_transform.setPosition(m_transform.getPosition() + m_velocity * delta);
}

//const Vector3& PhysicsObject::getPosition() const
//{
//	return m_position;
//}

const Vector3& PhysicsObject::getVelocity() const
{
	return m_velocity;
}

const Collider& PhysicsObject::getCollider() const
{
	//Vector3 translationAmount = m_position - m_oldPosition;
	//m_oldPosition = m_position;
	//m_collider->transform(translationAmount);
	return *m_collider;
}

void PhysicsObject::setVelocity(const Vector3& velocity)
{
	m_velocity = velocity;
}

PhysicsObject PhysicsObject::operator =(PhysicsObject other)
{
	char* temp[sizeof(PhysicsObject) / sizeof(char)];
	memcpy(temp, this, sizeof(PhysicsObject));
	memcpy(this, &other, sizeof(PhysicsObject));
	memcpy(&other, temp, sizeof(PhysicsObject));

	return *this;
}