#ifndef OCTREE_H
#define OCTREE_H

#include <set>

#include "../../Core/GameComponent.h"
#include "../../Core/Math3D.h"
#include "../Collider.h"
#include "../PhysicsObject.h"

const int MAX_OCTREE_DEPTH = 6;
const int MIN_BALLS_PER_OCTREE = 2;
const int MAX_BALLS_PER_OCTREE = 3;

class PhysicsObject;

struct BallPair
{
	PhysicsObject* ball1;
	PhysicsObject* ball2;
};

struct Partition
{
	Vector3 centre;
	Vector3 min;
	Vector3 max;
};

class Octree
{
public:
	static std::vector<Partition> partitions;

	Vector3 corner1; //min
	Vector3 corner2; //max
	Vector3 centre;

	Octree* children[2][2][2];
	bool hasChildren;
	std::set<PhysicsObject*> m_objects;
	int depth;
	int numBalls;

	Octree(Vector3 c1, Vector3 c2, int d);

	void fileBall(PhysicsObject* physicsObject, bool addBall);
	void haveChildren();
	void collectBalls(std::set<PhysicsObject*>& physicsObjectSet);
	void destroyChildren();
	void remove(PhysicsObject* physicsObject);
	void add(PhysicsObject* physicsObject);
	void ballMoved(PhysicsObject* physicsObject, Vector3 oldPosition);
	void potentialBallBallCollisions(std::vector<BallPair>& collisions);

	void advanceState(std::vector<PhysicsObject*>& objects, float delta);
	void moveObjects(std::vector<PhysicsObject*>& objects, float delta);
};

//void moveBalls(std::vector<int*>& objs, Octree* octree, float delta)
//{
//	//Ball* ball = objs[0]->b;
//	//Vector3 oldPosition = ball->pos;
//	//ball->pos += ball->v * delta;
//	//octree->ballMoved(ball, oldPosition);
//}
//
//void advance(std::vector<Ball*>& objs, Octree* octree, float delta)
//{
//	//moveBalls(objs, octree, delta);
//	//handleCollision
//}
/*

//Moves all of the balls by their velocity times dt
void moveBalls(vector<Ball*> &balls, Octree* octree, float dt) {
for(unsigned int i = 0; i < balls.size(); i++) {
Ball* ball = balls[i];
Vec3f oldPos = ball->pos;
ball->pos += ball->v * dt;
octree->ballMoved(ball, oldPos);
}
}

void advance(vector<Ball*> &balls,
Octree* octree,
float t,
float &timeUntilUpdate) {
while (t > 0) {
if (timeUntilUpdate <= t) {
moveBalls(balls, octree, timeUntilUpdate);
performUpdate(balls, octree);
t -= timeUntilUpdate;
timeUntilUpdate = TIME_BETWEEN_UPDATES;
}
else {
moveBalls(balls, octree, t);
timeUntilUpdate -= t;
t = 0;
}
}
}
*/

#endif