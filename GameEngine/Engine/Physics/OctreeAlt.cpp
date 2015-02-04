#include "Octree.h"

void OctreeAlt::fileObject(PhysicsObject* object, const Vector3& position, bool addObject)
{
	float s = object->getTransform()->getScale().getX();

	for(int x = 0; x < 2; x++)
	{
		if(x == 0)
		{
			if(position[0] - s > m_centre[0])
			{
				continue;
			}
		}
		else if(position[0] + s < m_centre[0])
		{
			continue;
		}

		for(int y = 0; y < 2; y++)
		{
			if(y == 0)
			{
				if(position[1] - s > m_centre[1])
				{
					continue;
				}
			}
			else if(position[1] + s < m_centre[1])
			{
				continue;
			}

			for(int z = 0; z < 2;z++)
			{
				if(z == 0)
				{
					if(position[2] - s > m_centre[2])
					{
						continue;
					}
				}
				else if(position[2] + s < m_centre[2])
				{
					continue;
				}

				if(addObject)
				{
					m_children[x][y][z]->add(object);
				}
				else
				{
					m_children[x][y][z]->remove(object, position);
				}
			}
		}
	}
}

void OctreeAlt::haveChildren()
{
	for(int x = 0; x < 2; x++)
	{
		float minX, maxX;

		if(x == 0)
		{
			minX = m_corner1[0];
			maxX = m_centre[0];
		}
		else
		{
			minX = m_centre[0];
			maxX = m_corner2[0];
		}

		for(int y = 0; y < 2; y++)
		{
			float minY, maxY;

			if(y == 0)
			{
				minY = m_corner1[1];
				maxY = m_centre[1];
			}
			else
			{
				minY = m_centre[1];
				maxY = m_corner2[1];
			}

			for(int z = 0; z < 2; z++)
			{
				float minZ, maxZ;

				if(z == 0)
				{
					minZ = m_corner1[2];
					maxZ = m_centre[2];
				}
				else
				{
					minZ = m_centre[2];
					maxZ = m_corner2[2];
				}

				m_children[x][y][z] = new OctreeAlt(Vector3(minX, minY, minZ), Vector3(maxX, maxY, maxZ), m_depth + 1);
			}
		}
	}

	for(std::set<PhysicsObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		PhysicsObject* o = *it;
		fileObject(o, o->getTransform()->getPosition(), true);
	}

	m_objects.clear();
	m_hasChildren = true;
}

void OctreeAlt::collectObjects(std::set<PhysicsObject*>& set)
{
	if(m_hasChildren)
	{
		for(int x = 0; x < 2; x++)
		{
			for(int y = 0; y < 2; y++)
			{
				for(int z = 0; z < 2; z++)
				{
					m_children[x][y][z]->collectObjects(set);
				}
			}
		}
	}
	else
	{
		for(std::set<PhysicsObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
		{
			PhysicsObject* p = *it;
			set.insert(p);
		}
	}
}

void OctreeAlt::killChildren()
{
	collectObjects(m_objects);

	for(int x = 0; x < 2; x++)
	{
		for(int y = 0; y < 2; y++)
		{
			for(int z = 0; z < 2; z++)
			{
				delete m_children[x][y][z];
			}
		}
	}

	m_hasChildren = false;
}

void OctreeAlt::remove(PhysicsObject* object, const Vector3& position)
{
	m_numObjects--;

	if(m_hasChildren && m_numObjects < MIN_OBJ_PER_OCTAN)
	{
		killChildren();
	}

	if(m_hasChildren)
	{
		fileObject(object, position, false);
	}
	else
	{
		m_objects.erase(object);
	}
}


OctreeAlt::OctreeAlt(const Vector3& min, const Vector3& max, int depth)
{
	m_corner1 = min;
	m_corner2 = max;
	m_centre = (min + max) / 2.0f;
	m_depth = depth;
	m_numObjects = 0;
	m_hasChildren = false;
}

OctreeAlt::~OctreeAlt()
{
	if(m_hasChildren)
	{
		killChildren();
	}
}

void OctreeAlt::add(PhysicsObject* object)
{
	m_numObjects++;

	if(!m_hasChildren && m_depth < MAX_OCTREE_DEPTH &&
	   m_numObjects > MAX_OBJ_PER_OCTAN)
	{
		haveChildren();
	}

	if(m_hasChildren)
	{
		fileObject(object, object->getTransform()->getPosition(), true);
	}
	else
	{
		m_objects.insert(object);
	}
}

void OctreeAlt::remove(PhysicsObject* object)
{
	remove(object, object->getTransform()->getPosition());
}

void OctreeAlt::objectMoved(PhysicsObject* object, const Vector3& oldPosition)
{
	remove(object, oldPosition);
	add(object);
}

void OctreeAlt::potentialCollisions()
{
	if(m_hasChildren)
	{
		for(int x = 0; x < 2; x++)
		{
			for(int y = 0; y < 2; y++)
			{
				for(int z = 0; z < 2; z++)
				{
					m_children[x][y][z]->potentialCollisions();
				}
			}
		}
	}
	else
	{
		for(std::set<PhysicsObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
		{
			PhysicsObject* p1 = *it;

			for(std::set<PhysicsObject*>::iterator it2 = m_objects.begin(); it2 != m_objects.end(); it2++)
			{
				PhysicsObject* p2 = *it2;

				if(p1 < p2)
				{
					CollisionData_ data;
					p1->getCollider()->collide(*p2, data);
				}
			}
		}
	}
}
