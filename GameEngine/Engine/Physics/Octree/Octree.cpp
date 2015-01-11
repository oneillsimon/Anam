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

void Octree::fileBall(Ball* ball, Vector3 position, bool addBall)
{
	for(int x = 0; x < 2; x++)
	{
		if(x == 0)
		{
			if(position.getX() - ball->r > centre.getX())
			{
				continue;
			}
			else if(position.getX() + ball->r < centre.getX())
			{
				continue;
			}

			for(int y = 0; y < 2; y++)
			{
				if(y == 0)
				{
					if(position.getY() - ball->r > centre.getY())
					{
						continue;
					}
					else if(position.getY() + ball->r < centre.getY())
					{
						continue;
					}

					for(int z = 0; z < 2; z++)
					{
						if(z == 0)
						{
							if(position.getZ() - ball->r > centre.getZ())
							{
								continue;
							}
							else if(position.getZ() + ball->r < centre.getZ())
							{
								continue;
							}

							// Add or remove the ball
							if(addBall)
							{
								children[x][y][z]->add(ball);
							}
							else
							{
								children[x][y][z]->remove(ball, position);
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

	for(std::set<Ball*>::iterator it = balls.begin(); it != balls.end(); it++)
	{
		Ball* ball = *it;
		fileBall(ball, ball->pos, true);
	}
	balls.clear();
	hasChildren = true;
}

void Octree::collectBalls(std::set<Ball*>& bs)
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
		for(std::set<Ball*>::iterator it = balls.begin(); it != balls.end(); it++)
		{
			Ball* ball = *it;
			bs.insert(ball);
		}
	}
}

void Octree::destroyChildren()
{
	collectBalls(balls);

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

void Octree::remove(Ball* ball, Vector3 position)
{
	numBalls--;

	if(hasChildren && numBalls < MIN_BALLS_PER_OCTREE)
	{
		destroyChildren();
	}

	if(hasChildren)
	{
		fileBall(ball, position, false);
	}
	else
	{
		balls.erase(ball);
	}
}

void Octree::remove(Ball* ball)
{
	remove(ball, ball->pos);

}

void Octree::add(Ball* ball)
{
	numBalls++;

	if(!hasChildren && depth < MAX_BALLS_PER_OCTREE &&
		numBalls > MAX_BALLS_PER_OCTREE)
	{
		haveChildren();
	}

	if(hasChildren)
	{
		fileBall(ball, ball->pos, true);
	}
	else
	{
		balls.insert(ball);
	}
}

void Octree::ballMoved(Ball* ball, Vector3 oldPosition)
{
	remove(ball, oldPosition);
	add(ball);
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
		for(std::set<Ball*>::iterator it = balls.begin(); it != balls.end(); it++)
		{
			Ball* ball1 = *it;

			for(std::set<Ball*>::iterator it2 = balls.begin(); it2 != balls.end(); it2++)
			{
				Ball* ball2 = *it2;

				if(ball1 < ball2)
				{
					BallPair bp;
					bp.ball1 = ball1;
					bp.ball2 = ball2;
					collisions.push_back(bp);
				}
			}
		}
	}
}