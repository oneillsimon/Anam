#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <set>

#include "Collider.h"
#include "PhysicsObject.h"

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
	static std::vector<PhysicsObject*> m_allObjects;

	const int MIN_OBJ_COUNT = 1;
	const int MAX_OBJ_COUNT = 1;
	const int MAX_DEPTH = 6;

	Vector3 m_centre;
	Vector3 m_halfExtents;

	int m_depth;
	Octree* m_parent;
	std::vector<Octree*> octans;
	bool m_hasChildren;

	std::set<PhysicsObject*> mm_objects;

	Octree(Octree* parent, int depth, const Vector3& centre, const Vector3& halfExtents);

	void addObject(PhysicsObject* object);
	void addToChild(PhysicsObject* object);
	void removeObject(PhysicsObject* object);
	void divideDown();
	void multiplyUp();
	void collectObjects(std::set<PhysicsObject*>& objects);
	void recalculate();
	bool isInside(const Octree& octree, const Vector3& position);
	void potentialCollisions();
	int countObjects();
	//void generateContacts(Collider& one, Collider& two, CollisionData_* data);
};

class OctreeAlt
{
public:
	static std::vector<Partition> partitions;

	const int MIN_OBJ_PER_OCTAN = 1;
	const int MAX_OBJ_PER_OCTAN = 8;
	const int MAX_OCTREE_DEPTH = 6;

	Vector3 m_corner1;
	Vector3 m_corner2;
	Vector3 m_centre;

	OctreeAlt* m_children[2][2][2];
	~OctreeAlt();
	bool m_hasChildren;
	std::set<PhysicsObject*> m_objects;
	int m_depth;
	int m_numObjects;

	void fileObject(PhysicsObject* object, const Vector3& position, bool addObject);
	void divideDown();
	void collectObjects(std::set<PhysicsObject*>& set);
	void killChildren();
	void remove(PhysicsObject* object, const Vector3& position);

	OctreeAlt(const Vector3& min, const Vector3& max, int depth);
	void add(PhysicsObject* object);
	void remove(PhysicsObject* object);
	void objectMoved(PhysicsObject* object, const Vector3& oldPosition);
	void potentialCollisions();
};

#endif