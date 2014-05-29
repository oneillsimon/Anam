#include "ReferenceCounter.h"

ReferenceCounter::ReferenceCounter()
{
	referenceCount = 1;
}

void ReferenceCounter::addReference()
{
	referenceCount++;
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