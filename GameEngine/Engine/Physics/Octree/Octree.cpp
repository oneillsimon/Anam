#include "Octree.h"

Octree::Octree(Vector3 c1, Vector3 c2, int d)
{
	corner1 = c1;
	corner2 = c2;
	centre = (c1 + c2) / 2;
	depth = d;
	numBalls = 0;
	hasChildren = false;
}

void Octree::fileBall(PhysicsObject* physicsObject, bool addBall)
{
	Vector3 position = physicsObject->getTransform()->getPosition();

	for(int x = 0; x < 2; x++)
	{
		if(x == 0)
		{
			if(position.getX() - physicsObject->r > centre.getX())
			{
				continue;
			}
			else if(position.getX() + physicsObject->r < centre.getX())
			{
				continue;
			}

			for(int y = 0; y < 2; y++)
			{
				if(y == 0)
				{
					if(position.getY() - physicsObject->r > centre.getY())
					{
						continue;
					}
					else if(position.getY() + physicsObject->r < centre.getY())
					{
						continue;
					}

					for(int z = 0; z < 2; z++)
					{
						if(z == 0)
						{
							if(position.getZ() - physicsObject->r > centre.getZ())
							{
								continue;
							}
							else if(position.getZ() + physicsObject->r < centre.getZ())
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

	for(std::set<PhysicsObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		PhysicsObject* physicsObject = *it;
		fileBall(physicsObject, true);
	}

	objects.clear();
	hasChildren = true;
}

void Octree::collectBalls(std::set<PhysicsObject*>& bs)
{
	if(hasChildren)
	{
		for(int x = 0; x < 2; x++)
		{
			for(int y = 0; y < 2; y++)
			{
				for(int z = 0; z < 2; z++)
				{
					children[x][y][z]->collectBalls(bs);
				}
			}
		}
	}
	else
	{
		for(std::set<PhysicsObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		{
			PhysicsObject* physicsObject = *it;
			bs.insert(physicsObject);
		}
	}
}

void Octree::destroyChildren()
{
	collectBalls(objects);

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
		fileBall(physicsObject, false);
	}
	else
	{
		objects.erase(physicsObject);
	}
}

void Octree::add(PhysicsObject* physicsObject)
{
	numBalls++;

	if(!hasChildren && depth < MAX_BALLS_PER_OCTREE &&
		numBalls > MAX_BALLS_PER_OCTREE)
	{
		haveChildren();
	}

	if(hasChildren)
	{
		fileBall(physicsObject, true);
	}
	else
	{
		objects.insert(physicsObject);
	}
}

void Octree::ballMoved(PhysicsObject* physicsObject, Vector3 oldPosition)
{
	remove(physicsObject);
	add(physicsObject);
}

void Octree::potentialBallBallCollisions(std::vector<BallPair>& collisions)
{
	if(hasChildren)
	{
		for(int x = 0; x < 2; x++)
		{
			for(int y = 0; y < 2; y++)
			{
				for(int z = 0; z < 2; z++)
				{
					children[x][y][z]->potentialBallBallCollisions(collisions);
				}
			}
		}
	}
	else
	{
		for(std::set<PhysicsObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		{
			PhysicsObject* obj1 = *it;

			for(std::set<PhysicsObject*>::iterator it2 = objects.begin(); it2 != objects.end(); it2++)
			{
				PhysicsObject* obj2 = *it2;

				if(obj1 < obj2)
				{
					BallPair bp;
					bp.ball1 = obj1;
					bp.ball2 = obj2;
					collisions.push_back(bp);
				}
			}
		}
	}
}

void Octree::advanceState(std::vector<PhysicsObject*>& objects, float delta)
{
	moveObjects(objects, delta);
}

void Octree::moveObjects(std::vector<PhysicsObject*>& objects, float delta)
{
	PhysicsObject* physicsObject = objects[0];
	Vector3 oldPosition = physicsObject->getTransform()->getPosition();
	physicsObject->getTransform()->setPosition(oldPosition + (physicsObject->velocity * delta));
	ballMoved(physicsObject, oldPosition);
}