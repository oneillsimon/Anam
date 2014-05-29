#pragma once

class ReferenceCounter
{
private:
	int referenceCount;

public:
	ReferenceCounter();

	void addReference();
	bool removeReference();

	int getReferenceCount();
};