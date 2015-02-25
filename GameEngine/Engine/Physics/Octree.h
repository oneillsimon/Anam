#ifndef OCTREE_H
#define OCTREE_H

#define MAX_CONTACTS 1024

#include <vector>
#include <set>

#include "Collider.h"
#include "PhysicsObject.h"
#include "Collision.h"

class Octree
{
private:
	const int MIN_OBJ_PER_OCTAN = 1;
	const int MAX_OBJ_PER_OCTAN = 4;
	const int MAX_OCTREE_DEPTH = 6;

	Vector3 m_min;
	Vector3 m_max;
	Vector3 m_centre;

	Octree* m_children[2][2][2];
	bool m_hasChildren;
	std::set<PhysicsObject*> m_objects;
	int m_depth;
	int m_numObjects;

	void fileObject(PhysicsObject* object, const Vector3& position, bool addObject);
	void divideDown();
	void collectObjects(std::set<PhysicsObject*>& set);
	void killChildren();
	void remove(PhysicsObject* object, const Vector3& position);

public:
	Octree(const Vector3& min, const Vector3& max, int depth);
	~Octree();
	void add(PhysicsObject* object);
	void remove(PhysicsObject* object);
	void objectMoved(PhysicsObject* object, const Vector3& oldPosition);
	void potentialCollisions(CollisionData* data);
	void generateContacts(Collider& one, Collider& two, CollisionData* data);
	bool isInside(const Vector3& point, Octree* octree);

	Octree* getChild(int x, int y, int z) const;

	Vector3 getMinExtents() const;
	Vector3 getMaxExtents() const;
	Vector3 getCentre() const;

	bool getHasChildren();
};

#endif