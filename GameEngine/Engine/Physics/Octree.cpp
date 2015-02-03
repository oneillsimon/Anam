#include "Octree.h"

std::vector<Partition> Octree::partitions = std::vector<Partition>();
std::vector<PhysicsObject*> Octree::m_allObjects = std::vector<PhysicsObject*>();

Octree::Octree(Octree* parent, int depth, const Vector3& centre, const Vector3& halfExtents) :
m_parent(parent),
m_depth(depth),
m_centre(centre),
m_halfExtents(halfExtents)
{
	m_hasChildren = false;
	
	
	Partition p;
	p.centre = centre;
	p.max = halfExtents;
	p.min = halfExtents * -1;
	partitions.push_back(p);
}

void Octree::addObject(PhysicsObject* object)
{
	if(!m_hasChildren)
	{
		if(isInside(*this, object->getTransform()->getPosition()))
		{
			int count = countObjects();
			if(count >= MAX_OBJ_COUNT)
			{
				divideDown();

				std::set<PhysicsObject*> objects;
				collectObjects(objects);

				for(std::set<PhysicsObject*>::iterator it = objects.begin(); it != objects.end(); it++)
				{
					addObject(*it);
				}
			}
			else
			{
				mm_objects.insert(object);
			}
		}
	}
	else
	{
		for(int i = 0; i < octans.size(); i++)
		{
			octans[i]->addObject(object);
		}
	}
}

void Octree::divideDown()
{
	float halfX = m_halfExtents.getX() / 2.0f;
	float halfY = m_halfExtents.getY() / 2.0f;
	float halfZ = m_halfExtents.getZ() / 2.0f;

	octans.push_back(new Octree(this, m_depth + 1, Vector3(m_centre.getX() + halfX, m_centre.getY() + halfY, m_centre.getZ() + halfZ), m_halfExtents / 2.0f));
	octans.push_back(new Octree(this, m_depth + 1, Vector3(m_centre.getX() - halfX, m_centre.getY() + halfY, m_centre.getZ() + halfZ), m_halfExtents / 2.0f));
	octans.push_back(new Octree(this, m_depth + 1, Vector3(m_centre.getX() + halfX, m_centre.getY() + halfY, m_centre.getZ() - halfZ), m_halfExtents / 2.0f));
	octans.push_back(new Octree(this, m_depth + 1, Vector3(m_centre.getX() - halfX, m_centre.getY() + halfY, m_centre.getZ() - halfZ), m_halfExtents / 2.0f));

	octans.push_back(new Octree(this, m_depth + 1, Vector3(m_centre.getX() + halfX, m_centre.getY() - halfY, m_centre.getZ() + halfZ), m_halfExtents / 2.0f));
	octans.push_back(new Octree(this, m_depth + 1, Vector3(m_centre.getX() - halfX, m_centre.getY() - halfY, m_centre.getZ() + halfZ), m_halfExtents / 2.0f));
	octans.push_back(new Octree(this, m_depth + 1, Vector3(m_centre.getX() + halfX, m_centre.getY() - halfY, m_centre.getZ() - halfZ), m_halfExtents / 2.0f));
	octans.push_back(new Octree(this, m_depth + 1, Vector3(m_centre.getX() - halfX, m_centre.getY() - halfY, m_centre.getZ() - halfZ), m_halfExtents / 2.0f));

	m_hasChildren = true;
}

void Octree::multiplyUp()
{
	for(int i = 0; i < 8; i++)
	{
		octans.pop_back();
	}

	m_hasChildren = false;
}

void Octree::collectObjects(std::set<PhysicsObject*>& objects)
{
	for(std::set<PhysicsObject*>::iterator it = mm_objects.begin(); it != mm_objects.end(); it++)
	{
		objects.insert(*it);
	}
}

void Octree::recalculate()
{
	if(!m_hasChildren)
	{
		int count = countObjects();
		printf("count: %d\n", count);
		if(count < MIN_OBJ_COUNT)
		{
			printf("multiplying\n");
			m_parent->multiplyUp();
		}
	}
	else
	{
		for(int i = 0; i < octans.size(); i++)
		{
			octans[i]->recalculate();
		}
	}
}

bool Octree::isInside(const Octree& octree, const Vector3& position)
{
	Vector3 min = octree.m_centre - octree.m_halfExtents;
	Vector3 max = octree.m_centre + octree.m_halfExtents;

	return ((position.getX() >= min.getX() && position.getX() <= max.getX()) &&
		(position.getY() >= min.getY() && position.getY() <= max.getY()) &&
		(position.getZ() >= min.getZ() && position.getZ() <= max.getZ()));
}

void Octree::potentialCollisions()
{
	if(mm_objects.size() <= 0)
	{
		return;
	}

	if(!m_hasChildren)
	{
		for(std::set<PhysicsObject*>::iterator it1 = mm_objects.begin(); it1 != mm_objects.end(); it1++)
		{
			PhysicsObject* obj1 = *it1;

			for(std::set<PhysicsObject*>::iterator it2 = mm_objects.begin(); it2 != mm_objects.end(); it2++)
			{
				PhysicsObject* obj2 = *it2;

				if(obj1 < obj2)
				{
					CollisionData_ data;
					obj1->getCollider()->collide(*obj2, data);
				}
			}
		}
	}
	else
	{
		for(int i = 0; i < octans.size(); i++)
		{
			octans[i]->potentialCollisions();
		}
	}
}

int Octree::countObjects()
{
	int count = 0;

	for(int i = 0; i < m_allObjects.size(); i++)
	{
		if(isInside(*this, m_allObjects[i]->getTransform()->getPosition()))
		{
			count++;
		}
	}

	for(std::set<PhysicsObject*>::iterator i = mm_objects.begin(); i != mm_objects.end(); i++)
	{
		PhysicsObject* o = *i;
		o->getTransform()->setScale(count);
	}

	return count;
}