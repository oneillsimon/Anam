#ifndef INTERSECTIONDATA_H
#define INTERSECTIONDATA_H

#include "../Core/Math3D.h"
//#include "../Physics/PhysicsObject.h"

class IntersectionData
{
private:
	const bool m_doesIntersect;
	const Vector3 m_direction;

public:
	IntersectionData(const bool doesIntersect, const Vector3 direction);

	bool getDoesIntersect() const;
	Vector3 getDirection() const;
	float getDistance() const;
};

#endif