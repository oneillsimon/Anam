#include "IntersectionData.h"

IntersectionData::IntersectionData(const bool doesIntersect, const Vector3 direction) :
	m_doesIntersect(doesIntersect),
	m_direction(direction)
{
}

bool IntersectionData::getDoesIntersect() const
{
	return m_doesIntersect;
}

Vector3 IntersectionData::getDirection() const
{
	return m_direction.normalised();
}

float IntersectionData::getDistance() const
{
	return m_direction.length();
}