#include <thread>

#include "PhysicsEngine.h"

//int coll::gSortAxis = 0;

namespace coll
{
	//gSortAxis = 0;
}
int PhysicsEngine::gSortAxis = 0;
//static int coll::gSortAxis = 0;

int cmpPCs(const void* a, const void* b)
{
	Collider* aa = (*(Collider**)a);
	Collider* bb = (*(Collider**)b);
	Vector3 minExtentsA = aa->getMin();
	Vector3 minExtentsB = bb->getMax();
	float minA = minExtentsA[PhysicsEngine::gSortAxis];
	float minB = minExtentsB[PhysicsEngine::gSortAxis];

	if(minA < minB)
	{
		return -1;
	}

	if(minA > minB)
	{
		return 1;
	}

	return 0;
}

PhysicsEngine::PhysicsEngine() :
resolver(MAX_CONTACTS)
{
	float size = 12.0f;
	m_tree = new Octree(Vector3(-size, -size, -size), Vector3(size, size, size), 1);

	cData.setContactArray(contacts);
	cData.reset(MAX_CONTACTS);
}

void PhysicsEngine::addComponent(PhysicsComponent* component)
{
	//m_components.push_back(component);
	gPCArray[currentIndex] = component->getCollider();
	currentIndex++;
	//m_tree->add(component);
}

void PhysicsEngine::simulate(float delta)
{
	sortAndSweep();
//	printf("%d\n", cData.getContactCount());
	
	resolver.resolveContacts(cData.getContactArray(), cData.getContactCount(), delta);
	cData.reset(MAX_CONTACTS);
	//if(m_components.empty())
	//{
	//	return;
	//}
	//
	//cData.reset(MAX_CONTACTS);
	//
	//ProfileTimers::physicsTimer.startInvocation();
	//ProfileTimers::physRefTimer.startInvocation();
	//updateComponentReferences(m_components, m_tree, delta);
	//ProfileTimers::physRefTimer.stopInvocation();
	//m_tree->potentialCollisions(&cData);
	//resolver.resolveContacts(cData.getContactArray(), cData.getContactCount(), delta);
	//ProfileTimers::physicsTimer.stopInvocation();
}

void PhysicsEngine::updateComponentReferences(std::vector<PhysicsComponent*>& components, Octree* octree, float delta)
{
	for(unsigned i = 0; i < components.size(); i++)
	{
		if(components[i]->getTransform()->hasChanged())
		{
			octree->objectMoved(components[i], components[i]->getTransform()->getPosition());
		}
	}
}

void PhysicsEngine::sortAndSweep()
{
	std::qsort(gPCArray, currentIndex, sizeof(Collider*), cmpPCs);
	
	float s[3] = { 0.0f, 0.0f, 0.0f }, s2[3] = { 0.0f, 0.0f, 0.0f }, v[3];

	for(int i = 0; i < currentIndex; i++)
	{
		Vector3 p = (gPCArray[i]->getMin() + gPCArray[i]->getMax()) * 0.5f;

		for(int c = 0; c < 3; c++)
		{
			s[c] += p[c];
			s2[c] += p[c] * p[c];
		}

		for(int j = i + 1; j < currentIndex; j++)
		{
			if(gPCArray[j]->getMin()[gSortAxis] > gPCArray[i]->getMax()[gSortAxis])
			{
				printf("%.4f, %.4f, %d\n", gPCArray[j]->getMin()[gSortAxis], gPCArray[i]->getMax()[gSortAxis], gSortAxis);
				break;
			}

			cData.setFriction(0.9f);
			cData.setRestitution(0.1f);
			cData.setTolerance(0.1f);

			//if(IntersectionTests::colliderAndCollider(*gPCArray[i], *gPCArray[j]))
			{
				gPCArray[i]->collide(*gPCArray[j], cData);
			}
		}
	}

	for(int c = 0; c < 3; c++)
	{
		v[c] = s2[c] - s[c] * s[c] / currentIndex;
	}

	gSortAxis = 0;
	if(v[1] > v[0]) gSortAxis = 1;
	if(v[2] > v[gSortAxis]) gSortAxis = 2;
}