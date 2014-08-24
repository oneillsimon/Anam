#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Core/ReferenceCounter.h"
#include "IntersectionData.h"

class Collider : public ReferenceCounter
{
private:
	int m_type;

public:
	enum
	{
		TYPE_SPHERE,
		TYPE_AABB,
		TYPE_SIZE
	};

	Collider(int type);

	IntersectionData intersect(const Collider& collider) const;
	virtual void transform(const Vector3& translation) {};

	int getType() const;

	virtual Vector3 getCentre() const;
};

#endif