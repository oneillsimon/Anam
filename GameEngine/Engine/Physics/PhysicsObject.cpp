#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(RigidBody* rigidBody, const Vector3& position, const Quaternion& rotation, float scale) :
	m_rigidBody(rigidBody),
	GameObject(position, rotation, scale)
{
	addComponent(m_rigidBody);
}