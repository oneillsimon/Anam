#include "IntersectionData.h"

IntersectionData::IntersectionData(bool doesIntersect, float distance)
{
	m_doesIntersect = doesIntersect;
	m_distance = distance;
}

bool IntersectionData::getDoesIntersect() const
{
	return m_doesIntersect;
}

float IntersectionData::getDistance() const
{
	return m_distance;
}