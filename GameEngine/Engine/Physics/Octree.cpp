#include "Octree.h"
#include "../Core/Profiling.h"

void Octree::fileObject(PhysicsComponent* component, const Vector3& position, bool addObject)
{
	float s = component->getCollider()->getExtents().max();

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
					m_children[x][y][z]->add(component);
				}
				else
				{
					m_children[x][y][z]->remove(component, position);
				}
			}
		}
	}
}

void Octree::divideDown()
{
	m_children[0][0][0] = new Octree(Vector3(m_min[0], m_min[1], m_min[2]), Vector3(m_centre[0], m_centre[1], m_centre[2]), m_depth + 1);
	m_children[1][0][0] = new Octree(Vector3(m_centre[0], m_min[1], m_min[2]), Vector3(m_max[0], m_centre[1], m_centre[2]), m_depth + 1);
	m_children[0][1][0] = new Octree(Vector3(m_min[0], m_centre[2], m_min[2]), Vector3(m_centre[0], m_max[1], m_centre[2]), m_depth + 1);
	m_children[1][1][0] = new Octree(Vector3(m_centre[0], m_centre[2], m_min[2]), Vector3(m_max[0], m_max[1], m_centre[2]), m_depth + 1);

	m_children[0][0][1] = new Octree(Vector3(m_min[0], m_min[1], m_centre[2]), Vector3(m_centre[0], m_centre[1], m_max[2]), m_depth + 1);
	m_children[1][0][1] = new Octree(Vector3(m_centre[0], m_min[1], m_centre[2]), Vector3(m_max[0], m_centre[1], m_max[2]), m_depth + 1);
	m_children[0][1][1] = new Octree(Vector3(m_min[0], m_centre[2], m_centre[2]), Vector3(m_centre[0], m_max[1], m_max[2]), m_depth + 1);
	m_children[1][1][1] = new Octree(Vector3(m_centre[0], m_centre[2], m_centre[2]), Vector3(m_max[0], m_max[1], m_max[2]), m_depth + 1);

	m_components.clear();
	m_numObjects = 0;
	m_hasChildren = true;

	for(std::set<PhysicsComponent*>::iterator it = m_components.begin(); it != m_components.end(); it++)
	{
		PhysicsComponent* c = *it;
		fileObject(c, c->getTransform()->getPosition(), true);
	}
}

void Octree::collectObjects(std::set<PhysicsComponent*>& set)
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
		for(std::set<PhysicsComponent*>::iterator it = m_components.begin(); it != m_components.end(); it++)
		{
			PhysicsComponent* c = *it;
			set.insert(c);
		}
	}
}

void Octree::killChildren()
{
	collectObjects(m_components);

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

void Octree::remove(PhysicsComponent* component, const Vector3& position)
{
	m_numObjects--;

	if(m_hasChildren && m_numObjects < MIN_OBJ_PER_OCTAN)
	{
		killChildren();
	}

	if(m_hasChildren)
	{
		fileObject(component, position, false);
	}
	else
	{
		m_components.erase(component);
	}
}

bool Octree::isInside(const Vector3& point, Octree* octree)
{
	Vector3 min = octree->m_min;
	Vector3 max = octree->m_max;

	return ((point[0] >= min[0] && point[0] <= max[0]) &&
		(point[1] >= min[1] && point[1] <= max[1]) &&
		(point[2] >= min[2] && point[2] <= max[2]));
}

Octree::Octree(const Vector3& min, const Vector3& max, int depth)
{
	m_min = min;
	m_max = max;
	m_centre = (min + max) / 2.0f;
	m_depth = depth;
	m_numObjects = 0;
	m_hasChildren = false;

	printf("Octree created at depth %d\n", m_depth);
}

Octree::~Octree()
{
	if(m_hasChildren)
	{
		killChildren();
	}
}

void Octree::add(PhysicsComponent* component)
{
	m_numObjects++;

	if(!m_hasChildren && m_depth < MAX_OCTREE_DEPTH &&
		m_numObjects > MAX_OBJ_PER_OCTAN)
	{
		divideDown();
	}

	if(m_hasChildren)
	{
		fileObject(component, component->getTransform()->getPosition(), true);
	}
	else
	{
		if(isInside(component->getTransform()->getPosition(), this))
		{
			m_components.insert(component);
		}
	}
}

void Octree::remove(PhysicsComponent* component)
{
	remove(component, component->getTransform()->getPosition());
}

void Octree::objectMoved(PhysicsComponent* component, const Vector3& oldPosition)
{
	remove(component, oldPosition);
	add(component);
}

void Octree::potentialCollisions(CollisionData* data)
{
	if(m_hasChildren)
	{
		for(int x = 0; x < 2; x++)
		{
			for(int y = 0; y < 2; y++)
			{
				for(int z = 0; z < 2; z++)
				{
					m_children[x][y][z]->potentialCollisions(data);
				}
			}
		}
	}
	else
	{
		for(std::set<PhysicsComponent*>::iterator it = m_components.begin(); it != m_components.end(); it++)
		{
			PhysicsComponent* c1 = *it;

			for(std::set<PhysicsComponent*>::iterator it2 = m_components.begin(); it2 != m_components.end(); it2++)
			{
				PhysicsComponent* c2 = *it2;

				if(c1 < c2)
				{
					generateContacts(*c2->getCollider(), *c1->getCollider(), data);
				}
			}
		}
	}
}

void Octree::generateContacts(Collider& one, Collider& two, CollisionData* data)
{
	data->setFriction(0.9f);
	data->setRestitution(1.0f);
	data->setTolerance(0.1f);

	one.collide(two, *data);
}

Octree* Octree::getChild(int x, int y, int z) const
{
	return m_children[x][y][z];
}

Vector3 Octree::getMinExtents() const
{
	return m_min;
}

Vector3 Octree::getMaxExtents() const
{
	return m_max;
}

Vector3 Octree::getCentre() const
{
	return m_centre;
}

bool Octree::getHasChildren()
{
	return m_hasChildren;
}