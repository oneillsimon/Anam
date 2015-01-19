#include "Octree.h"

std::vector<Partition> Octree::partitions = std::vector<Partition>();

Octree::Octree(Vector3 c1, Vector3 c2, int d)
{
	corner1 = c1;
	corner2 = c2;
	centre = (c1 + c2) / 2;
	depth = d;
	printf("octree created at depth %d\n", d);
	numBalls = 0;
	hasChildren = false;

	Partition p;
	p.centre = centre;
	p.min = corner1;
	p.max = corner2;

	partitions.push_back(p);
}

void Octree::fileCollider(PhysicsObject* physicsObject, bool addBall)
{
	Vector3 position = physicsObject->getTransform()->getPosition();
	Vector3 bounds = physicsObject->collider->m_radius;
	//Vector3 bounds = physicsObject->m_collider->getScale();

	for(int x = 0; x < 2; x++)
	{
		if(x == 0)
		{
			if(position.getX() - bounds.getX() > centre.getX())
			{
				continue;
			}
		}
		else if(position.getX() + bounds.getX() < centre.getX())
		{
			continue;
		}

		for(int y = 0; y < 2; y++)
		{
			if(y == 0)
			{
				if(position.getY() - bounds.getY() > centre.getY())
				{
					continue;
				}
			}
			else if(position.getY() + bounds.getY() < centre.getY())
			{
				continue;
			}

			for(int z = 0; z < 2; z++)
			{
				if(z == 0)
				{
					if(position.getZ() - bounds.getZ() > centre.getZ())
					{
						continue;
					}
				}
				else if(position.getZ() + bounds.getZ() < centre.getZ())
				{
					continue;
				}

				// Add or remove the ball
				if(addBall)
				{
					children[x][y][z]->add(physicsObject);
				}
				else
				{
					children[x][y][z]->remove(physicsObject);
				}
			}
		}
	}
}

void Octree::haveChildren()
{
	for(int x = 0; x < 2; x++)
	{
		float minX, maxX;

		if(x == 0)
		{
			minX = corner1.getX();
			maxX = centre.getX();
		}
		else
		{
			minX = centre.getX();
			maxX = corner2.getX();
		}

		for(int y = 0; y < 2; y++)
		{
			float minY, maxY;

			if(y == 0)
			{
				minY = corner1.getY();
				maxY = centre.getY();
			}
			else
			{
				minY = centre.getY();
				maxY = corner2.getY();
			}

			for(int z = 0; z < 2; z++)
			{
				float minZ, maxZ;

				if(z == 0)
				{
					minZ = corner1.getZ();
					maxZ = centre.getZ();
				}
				else
				{
					minZ = centre.getZ();
					maxZ = corner2.getZ();
				}

				children[x][y][z] = new Octree(Vector3(minX, minY, minZ),
					Vector3(maxX, maxY, maxZ), depth + 1);
			}
		}
	}

	for(std::set<PhysicsObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		PhysicsObject* physicsObject = *it;
		fileCollider(physicsObject, true);
	}

	m_objects.clear();
	hasChildren = true;
}

void Octree::collectColliders(std::set<PhysicsObject*>& bs)
{
	if(hasChildren)
	{
		for(int x = 0; x < 2; x++)
		{
			for(int y = 0; y < 2; y++)
			{
				for(int z = 0; z < 2; z++)
				{
					children[x][y][z]->collectColliders(bs);
				}
			}
		}
	}
	else
	{
		for(std::set<PhysicsObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
		{
			PhysicsObject* physicsObject = *it;
			bs.insert(physicsObject);
		}
	}
}

void Octree::destroyChildren()
{
	collectColliders(m_objects);

	for(int x = 0; x < 2; x++)
	{
		for(int y = 0; y < 2; y++)
		{
			for(int z = 0; z < 2; z++)
			{
				delete children[x][y][z];
			}
		}
	}

	hasChildren = false;
}

void Octree::remove(PhysicsObject* physicsObject)
{
	numBalls--;

	if(hasChildren && numBalls < MIN_BALLS_PER_OCTREE)
	{
		destroyChildren();
	}

	if(hasChildren)
	{
		fileCollider(physicsObject, false);
	}
	else
	{
		m_objects.erase(physicsObject);
	}
}

void Octree::add(PhysicsObject* physicsObject)
{
	numBalls++;

	if(!hasChildren && depth < MAX_OCTREE_DEPTH &&
		numBalls > MAX_BALLS_PER_OCTREE)
	{
		haveChildren();
	}

	if(hasChildren)
	{
		fileCollider(physicsObject, true);
	}
	else
	{
		m_objects.insert(physicsObject);
	}
}

void Octree::refreshObject(PhysicsObject* physicsObject)
{
	remove(physicsObject);
	add(physicsObject);
}

void Octree::potentialCollisions(CollisionData* data)
{
	if(hasChildren)
	{
		for(int x = 0; x < 2; x++)
		{
			for(int y = 0; y < 2; y++)
			{
				for(int z = 0; z < 2; z++)
				{
					children[x][y][z]->potentialCollisions(data);
				}
			}
		}
	}
	else
	{
		//data->reset(256);

		for(std::set<PhysicsObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
		{
			PhysicsObject* obj1 = *it;

			for(std::set<PhysicsObject*>::iterator it2 = m_objects.begin(); it2 != m_objects.end(); it2++)
			{
				PhysicsObject* obj2 = *it2;

				if(obj1 < obj2)
				{
					generateContacts(*obj1->collider, *obj2->collider, data);
				}
			}
		}
	}
}

void Octree::refreshObjects(std::vector<PhysicsObject*>& objects)
{
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		refreshObject(objects[i]);
	}
}

void Octree::generateContacts(const CollisionSphere& one, const CollisionSphere& two, CollisionData* data)
{
	data->reset(256);
	data->m_friction = 1.9f;
	data->m_restitution = 100.0f;
	data->m_tolerance = 0.1f;

	CollisionDetector::sphereAndSphere(one, two, data);
}