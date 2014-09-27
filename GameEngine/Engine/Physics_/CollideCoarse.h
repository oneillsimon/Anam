//#ifndef COLLIDECOARSE_H
//#define COLLIDECOARSE_H
//
//#include "RigidBody.h"
//
//class BoundingSphere_
//{
//private:
//	Vector3 m_centre;
//	float m_radius;
//
//public:
//	BoundingSphere_(const Vector3& centre, float radius);
//	BoundingSphere_(const BoundingSphere_& one, const BoundingSphere_& two);
//
//	bool overlaps(const BoundingSphere_* other) const;
//	float getGrowth(const BoundingSphere_& other) const;
//	float getSize() const;
//};
//
//class PotentialContact
//{
//private:
//	RigidBody* m_bodies[2];
//};
//
//template<class BoundingVolumeClass>
//class BVHNode
//{
//private:
//	BVHNode* m_parent;
//	BVHNode* m_children[2];
//	BoundingVolumeClass m_volume;
//	RigidBody* m_body;
//
//public:
//	BVHNode(BVHNode* parent, const BoundingVolumeClass& volume, RigidBody* body = 0);
//	~BVHNode<BoundingVolumeClass>();
//
//	bool isLeaf() const;
//
//	unsigned getPotentialContacts(PotentialContact* contacts, unsigned limit) const;
//	unsigned getPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other, PotentialContact* contacts, unsigned limit) const;
//	bool overlaps(const BVHNode<BoundingVolumeClass>* other) const;
//	void insert(RigidBody* newBody, const BoundingVolumeClass& newVolume);
//	void recalculateBoundingVolume(bool recurse = true);
//};
//
//#endif