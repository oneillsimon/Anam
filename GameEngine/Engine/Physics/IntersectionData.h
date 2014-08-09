#ifndef INTERSECTIONDATA_H
#define INTERSECTIONDATA_H

class IntersectionData
{
private:
	bool m_doesIntersect;
	float m_distance;

public:
	IntersectionData(const bool doesIntersect, const float distance);

	bool getDoesIntersect() const;
	float getDistance() const;
};

#endif