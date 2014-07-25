#include "ReferenceCounter.h"

ReferenceCounter::ReferenceCounter()
{
	referenceCount = 1;
}

void ReferenceCounter::addReference()
{
	this->referenceCount++;
}

bool ReferenceCounter::removeReference()
{
	referenceCount--;
	
	return referenceCount == 0;
}

int ReferenceCounter::getReferenceCount()
{
	return referenceCount;
}