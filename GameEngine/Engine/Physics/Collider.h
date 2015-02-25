#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Core/Math3D.h"
#include "RigidBody.h"

class CollisionData;
class RigidBody;

class Collider
{
protected:
	int m_type;
	RigidBody* m_body;

public:
	enum Type
	{
		SPHERE,
		BOX,
		PLANE,
	};

	Collider(int type);

	virtual void collide(Collider& collider, CollisionData& data) = 0;

	virtual Vector3 getExtents() = 0;

	RigidBody* getBody() const;
	int getType();

	void setBody(RigidBody* rigidBody);

	Vector3 getAxis(unsigned index) const;
};

class ColliderSphere : public Collider
{
public:
	ColliderSphere(float radius = 1.0f);
	float m_radius;

	virtual void collide(Collider& collider, CollisionData& data);
	virtual Vector3 getExtents();
};

class ColliderBox : public Collider
{
public:
	ColliderBox(const Vector3& halfSize = Vector3(1, 1, 1));
	Vector3 m_halfSize;

	virtual void collide(Collider& collider, CollisionData& data);
	virtual Vector3 getExtents();
};

class ColliderPlane : public Collider
{
public:
	ColliderPlane(const Vector3& normal = Vector3(0, 1, 0), float distance = 0.0f);
	Vector3 m_normal;
	float m_distance;

	virtual void collide(Collider& collider, CollisionData& data);
	virtual Vector3 getExtents();
};

#endif