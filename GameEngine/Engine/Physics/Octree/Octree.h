#ifndef OCTREE_H
#define OCTREE_H

#include <set>

#include "../../Core/Math3D.h"
#include "../PhysicsObject.h"

const int MAX_OCTREE_DEPTH = 6;
const int MIN_BALLS_PER_OCTREE = 3;
const int MAX_BALLS_PER_OCTREE = 6;

struct Ball
{
	Vector3 v;
	Vector3 pos;
	float r;
};

struct BallPair
{
	Ball* ball1;
	Ball* ball2;
};

class Octree
{
public:
	Vector3 corner1; //min
	Vector3 corner2; //max
	Vector3 centre;

	Octree* children[2][2][2];
	bool hasChildren;
	std::set<Ball*> balls;
	int depth;
	int numBalls;

	Octree(Vector3 c1, Vector3 c2, int d);

	void fileBall(Ball* ball, Vector3 position, bool addBall);
	void haveChildren();
	void collectBalls(std::set<Ball*>& bs);
	void destroyChildren();
	void remove(Ball* ball, Vector3 position);
	void remove(Ball* ball);
	void add(Ball* ball);
	void ballMoved(Ball* ball, Vector3 oldPosition);
	void potentialBallBallCollisions(std::vector<BallPair>& collisions);
};

#endif