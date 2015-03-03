#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <vector>

#include "../Core/Profiling.h"
#include "Octree.h"
#include "PhysicsComponent.h"
#include "Collision.h"

#define MAX_OBJECTS 256

//namespace coll
//{
	//static int gSortAxis;// = 0;
	int cmpPCs(const void* a, const void* b);
	Collider* determineColliderType(Collider* c);
//}

class PhysicsEngine
{
private:
	//std::vector<PhysicsComponent*> m_components;
	Collider* gPCArray[MAX_OBJECTS];
	int currentIndex = 0;
	
	CollisionData cData;
	ContactResolver resolver;
	Contact contacts[MAX_CONTACTS];

public:
	Octree* m_tree;
	PhysicsEngine();
	static int gSortAxis;
	void addComponent(PhysicsComponent* object);
	void simulate(float delta);
	void updateComponentReferences(std::vector<PhysicsComponent*>& components, Octree* octree, float delta);
	
	void sortAndSweep();

	//PhysicsObject* getObject(unsigned int index) const;
	//std::vector<PhysicsObject*>* getObjects();
};

//int coll::cmpPCs(const void* a, const void* b)
//{
//	Vector3 minExtentsA = (*(ColliderBox**)a)->getMin();
//	Vector3 minExtentsB = (*(ColliderBox**)b)->getMax();
//	float minA = minExtentsA[coll::gSortAxis];
//	float minB = minExtentsB[coll::gSortAxis];
//
//	if(minA < minB)
//	{
//		return -1;
//	}
//
//	if(minA > minB)
//	{
//		return 1;
//	}
//
//	return 0;
//}

#endif