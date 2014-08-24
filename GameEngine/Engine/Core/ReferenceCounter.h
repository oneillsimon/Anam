#ifndef REFERENCECOUNTER_H
#define REFERENCECOUNTER_H

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

#endif