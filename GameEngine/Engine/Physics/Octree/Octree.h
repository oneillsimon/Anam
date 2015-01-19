#ifndef OCTREE_H
#define OCTREE_H

#include <set>

#include "../../Core/GameComponent.h"
#include "../../Core/Math3D.h"
#include "../Collider.h"
#include "../PhysicsObject.h"
#include "../../Physics_/CollideFine.h"

const int MAX_OCTREE_DEPTH = 6;
const int MIN_BALLS_PER_OCTREE = 1;
const int MAX_BALLS_PER_OCTREE = 4;

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

	void fileCollider(PhysicsObject* physicsObject, bool addBall);
	void haveChildren();
	void collectColliders(std::set<PhysicsObject*>& physicsObjectSet);
	void destroyChildren();
	void remove(PhysicsObject* physicsObject);
	void add(PhysicsObject* physicsObject);
	void potentialCollisions(CollisionData* data);

	void refreshObject(PhysicsObject* physicsObject);
	void refreshObjects(std::vector<PhysicsObject*>& objects);
	void generateContacts(const CollisionSphere& one, const CollisionSphere& two, CollisionData* data);
};

#endif