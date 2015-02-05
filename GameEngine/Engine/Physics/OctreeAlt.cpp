#include "Octree.h"

std::vector<Partition> Octree::partitions = std::vector<Partition>();

void Octree::fileObject(PhysicsObject* object, const Vector3& position, bool addObject)
{
	float s = 0;//object->getTransform()->getScale().getX();

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

			for(int z = 0; z < 2; z++)
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

void Octree::divideDown()
{
	printf("division\n");
	m_children[0][0][0] = new Octree(Vector3(m_corner1[0], m_centre[1], m_corner1[2]), Vector3(m_centre[0], m_corner2[1], m_centre[2]), m_depth + 1);
	m_children[1][0][0] = new Octree(Vector3(m_centre[0], m_centre[1], m_corner1[2]), Vector3(m_corner2[0], m_corner2[1], m_centre[2]), m_depth + 1);
	m_children[0][1][0] = new Octree(Vector3(m_corner1[0], m_corner1[1], m_corner1[2]), Vector3(m_centre[0], m_centre[1], m_centre[2]), m_depth + 1);
	m_children[1][1][0] = new Octree(Vector3(m_centre[0], m_corner1[1], m_corner1[2]), Vector3(m_corner2[0], m_centre[1], m_centre[2]), m_depth + 1);
	
	m_children[0][0][1] = new Octree(Vector3(m_corner1[0], m_centre[1], m_centre[2]), Vector3(m_centre[0], m_corner2[1], m_corner2[2]), m_depth + 1);
	m_children[1][0][1] = new Octree(Vector3(m_centre[0], m_centre[1], m_centre[2]), Vector3(m_corner2[0], m_corner2[1], m_corner2[2]), m_depth + 1);
	m_children[0][1][1] = new Octree(Vector3(m_corner1[0], m_corner1[1], m_centre[2]), Vector3(m_centre[0], m_centre[1], m_corner2[2]), m_depth + 1);
	m_children[1][1][1] = new Octree(Vector3(m_centre[0], m_corner1[1], m_centre[2]), Vector3(m_corner2[0], m_centre[1], m_corner2[2]), m_depth + 1);

	m_objects.clear();
	m_numObjects = 0;
	m_hasChildren = true;

	for(std::set<PhysicsObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		PhysicsObject* o = *it;
		fileObject(o, o->getTransform()->getPosition(), true);
	}
}

void Octree::collectObjects(std::set<PhysicsObject*>& set)
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

void Octree::killChildren()
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

void Octree::remove(PhysicsObject* object, const Vector3& position)
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


Octree::Octree(const Vector3& min, const Vector3& max, int depth)
{
	m_corner1 = min;
	m_corner2 = max;
	m_centre = (min + max) / 2.0f;
	m_depth = depth;
	m_numObjects = 0;
	m_hasChildren = false;

	Partition p;
	p.centre = m_centre;
	p.min = min;
	p.max = max;

	partitions.push_back(p);

	printf("Octree created at depth %d\n", m_depth);
}

Octree::~Octree()
{
	if(m_hasChildren)
	{
		killChildren();
	}
}

void Octree::add(PhysicsObject* object)
{
	m_numObjects++;

	if(!m_hasChildren && m_depth < MAX_OCTREE_DEPTH &&
	   m_numObjects > MAX_OBJ_PER_OCTAN)
	{
		divideDown();
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

void Octree::remove(PhysicsObject* object)
{
	remove(object, object->getTransform()->getPosition());
}

void Octree::objectMoved(PhysicsObject* object, const Vector3& oldPosition)
{
	if(object->getTransform()->hasChanged())
	{
		remove(object, oldPosition);
		add(object);
	}
}

void Octree::potentialCollisions()
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
