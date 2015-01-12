#ifndef INTERSECTIONDATA_H
#define INTERSECTIONDATA_H

#include "../Core/Math3D.h"

class IntersectionData
{
private:
	const bool m_doesIntersect;
	const Vector3 m_direction;
	PhysicsObject* objects[2];

public:
	IntersectionData(const bool doesIntersect, const Vector3 direction);

	bool getDoesIntersect() const;
	Vector3 getDirection() const;
	float getDistance() const;
};

#endif