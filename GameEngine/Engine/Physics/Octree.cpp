#include "Octree.h"

std::vector<Partition> Octree::partitions = std::vector<Partition>();

Octree::Octree(int depth, const Vector3& centre, const Vector3& halfExtents) :
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
			if(mm_objects.size() >= MAX_OBJ_COUNT)
			{
				divideDown();

				std::set<PhysicsObject*> objects;
				collectObjects(objects);
				//mm_objects.clear();

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

	octans.push_back(new Octree(m_depth + 1, Vector3(m_centre.getX() + halfX, m_centre.getY() + halfY, m_centre.getZ() + halfZ), m_halfExtents / 2.0f));
	octans.push_back(new Octree(m_depth + 1, Vector3(m_centre.getX() - halfX, m_centre.getY() + halfY, m_centre.getZ() + halfZ), m_halfExtents / 2.0f));
	octans.push_back(new Octree(m_depth + 1, Vector3(m_centre.getX() + halfX, m_centre.getY() + halfY, m_centre.getZ() - halfZ), m_halfExtents / 2.0f));
	octans.push_back(new Octree(m_depth + 1, Vector3(m_centre.getX() - halfX, m_centre.getY() + halfY, m_centre.getZ() - halfZ), m_halfExtents / 2.0f));
	
	octans.push_back(new Octree(m_depth + 1, Vector3(m_centre.getX() + halfX, m_centre.getY() - halfY, m_centre.getZ() + halfZ), m_halfExtents / 2.0f));
	octans.push_back(new Octree(m_depth + 1, Vector3(m_centre.getX() - halfX, m_centre.getY() - halfY, m_centre.getZ() + halfZ), m_halfExtents / 2.0f));
	octans.push_back(new Octree(m_depth + 1, Vector3(m_centre.getX() + halfX, m_centre.getY() - halfY, m_centre.getZ() - halfZ), m_halfExtents / 2.0f));
	octans.push_back(new Octree(m_depth + 1, Vector3(m_centre.getX() - halfX, m_centre.getY() - halfY, m_centre.getZ() - halfZ), m_halfExtents / 2.0f));

	m_hasChildren = true;
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
	//if(mm_objects.size() < MIN_OBJ_COUNT)
	{
		//multiplyUp();
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

void Octree::potentialCollisions(CollisionData* data)
{
	if(mm_objects.size() <= 0)
	{
		return;
	}

	if(!m_hasChildren)
	{
		data->m_friction = 0.9f;
		data->m_restitution = 0.0f;
		data->m_tolerance = 0.1f;

		for(std::set<PhysicsObject*>::iterator it1 = mm_objects.begin(); it1 != mm_objects.end(); it1++)
		{
			PhysicsObject* obj1 = *it1;

			for(std::set<PhysicsObject*>::iterator it2 = mm_objects.begin(); it2 != mm_objects.end(); it2++)
			{
				PhysicsObject* obj2 = *it2;

				if(obj1 < obj2)
				{
					generateContacts(*obj1->m_collider, *obj2->m_collider, data);
				}
			}
		}
	}
	else
	{
		for(int i = 0; i < octans.size(); i++)
		{
			octans[i]->potentialCollisions(data);
		}
	}
}

void Octree::generateContacts(Collider& one, Collider& two, CollisionData* data)
{
	/*
	if(one.m_type == Collider::Type::SPHERE && two.m_type == Collider::Type::SPHERE)
	{
		CollisionDetector::sphereAndSphere(*dynamic_cast<Collider*>(&one), *dynamic_cast<Collider*>(&two), data);
	}
	else if(one.m_type == Collider::Type::SPHERE && two.m_type == Collider::Type::BOX)
	{
		CollisionDetector::boxAndSphere(*dynamic_cast<Collider*>(&two), *dynamic_cast<Collider*>(&one), data);
	}
	else if(one.m_type == Collider::Type::BOX && two.m_type == Collider::Type::SPHERE)
	{
		CollisionDetector::boxAndSphere(*dynamic_cast<Collider*>(&one), *dynamic_cast<Collider*>(&two), data);
	}
	else if(one.m_type == Collider::Type::BOX && two.m_type == Collider::Type::BOX)
	{
		CollisionDetector::boxAndBox(*dynamic_cast<Collider*>(&one), *dynamic_cast<Collider*>(&two), data);
	}
	else if(one.m_type == Collider::Type::SPHERE && two.m_type == Collider::Type::PLANE)
	{
		CollisionDetector::sphereAndHalfSpace(*dynamic_cast<Collider*>(&one), *dynamic_cast<Collider*>(&two), data);
	}
	else if(one.m_type == Collider::Type::PLANE && two.m_type == Collider::Type::SPHERE)
	{
		CollisionDetector::sphereAndHalfSpace(*dynamic_cast<Collider*>(&two), *dynamic_cast<Collider*>(&one), data);
	}
	else if(one.m_type == Collider::Type::BOX && two.m_type == Collider::Type::PLANE)
	{
		CollisionDetector::boxAndHalfSpace(*dynamic_cast<Collider*>(&one), *dynamic_cast<Collider*>(&two), data);
	}
	else if(one.m_type == Collider::Type::PLANE && two.m_type == Collider::Type::BOX)
	{
		CollisionDetector::boxAndHalfSpace(*dynamic_cast<Collider*>(&two), *dynamic_cast<Collider*>(&one), data);
	}
	*/
}