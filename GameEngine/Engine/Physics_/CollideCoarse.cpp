//#include "CollideCoarse.h"
//
//template<class BoundingVolumeClass>
//BVHNode<BoundingVolumeClass>::BVHNode(BVHNode* parent, const BoundingVolumeClass& volume, RigidBody* body)
//{
//	m_children[0] = 0;
//	m_children[1] = 0;
//}
//
//template<class BoundingVolumeClass>
//BVHNode<BoundingVolumeClass>::~BVHNode()
//{
//	if(m_parent)
//	{
//		BVHNode<BoundingVolumeClass>* sibling;
//
//		if(m_parent->m_children[0] == this)
//		{
//			sibling = m_parent->m_children[1];
//		}
//		else
//		{
//			sibling = m_parent->m_children[0];
//		}
//
//		m_parent->m_volume = sibling->m_volume;
//		m_parent->m_body = sibling->m_body;
//		m_parent->m_children[0] = sibling->m_children[0];
//		m_parent->m_children[1] = sibling->m_children[1];
//
//		sibling->m_parent = 0;
//		sibling->m_body = 0;
//		sibling->m_children[0] = 0;
//		sibling->m_children[1] = 0;
//		delete sibling;
//
//		m_parent->recalculateBoundingVolume();
//	}
//
//	if(m_children[0])
//	{
//		m_children[0]->m_parent = 0;
//		delete m_children[0];
//	}
//
//	if(m_children[1])
//	{
//		m_children[1]->m_parent = 0;
//		delete m_children[1];
//	}
//}
//
//template<class BoundingVolumeClass>
//bool BVHNode<BoundingVolumeClass>::isLeaf() const
//{
//	return m_body != 0;
//}
//
//template<class BoundingVolumeClass>
//unsigned BVHNode<BoundingVolumeClass>::getPotentialContacts(PotentialContact* contacts, unsigned limit) const
//{
//	if(isLeaf() || limit == 0)
//		{
//			return 0;
//		}
//	
//		return m_children[0]->getPotentialContactsWith(m_children[1], contacts, limit);
//}
//
//template<class BoundingVolumeClass>
//unsigned BVHNode<BoundingVolumeClass>::getPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other, PotentialContact* contacts, unsigned limit) const
//{
//if(!overlaps(other) || limit == 0)
//	{
//		return 0;
//	}
//
//	if(isLeaf() && other->isLeaf())
//	{
//		contacts->m_bodies[0] = m_body;
//		contacts->m_bodies[1] = other->m_body;
//		return 1;
//	}
//
//	if(other->isLeaf() || (!isLeaf() && m_volume->getSize() >= other->m_volume->getSize()))
//	{
//		unsigned count = m_children[0]->getPotentialContactsWith(other, contacts, limit);
//
//		if(limit > count)
//		{
//			return count + m_children[1]->getPotentialContactsWith(other, contacts + count, limit - count);
//		}
//		else
//		{
//			return count;
//		}
//	}
//	else
//	{
//		unsigned count = getPotentialContactsWith(other->m_children[0], contacts, limit);
//
//		if(limit > count)
//		{
//			return count + getPotentialContactsWith(other->m_children[1], contacts + count, limit - count);
//		}
//		else
//		{
//			return count;
//		}
//	}
//}
//
//template<class BoundingVolumeClass>
//bool BVHNode<BoundingVolumeClass>::overlaps(const BVHNode<BoundingVolumeClass>* other) const
//{
//	return m_volume->overlaps(other->m_volume);
//}
//
//template<class BoundingVolumeClass>
//void BVHNode<BoundingVolumeClass>::insert(RigidBody* newBody, const BoundingVolumeClass& newVolume)
//{
//	if(isLeaf())
//	{
//		m_children[0] = new BVHNode<BoundingVolumeClass>(this, m_volume, m_body);
//		m_children[1] = new BVHNode<BoundingVolumeClass>(this, newVolume, newBody);
//		this->m_body = 0;
//
//		recalculateBoundingVolume();
//	}
//	else
//	{
//		if(m_children[0]->m_volume.getGrowth(newVolume) < m_children[1]->m_volume.getGrowth(newVolume))
//		{
//			m_children[0]->insert(newBody, newVolume);
//		}
//		else
//		{
//			m_children[0]->insert(newBody, newVolume);
//		}
//	}
//}
//
//template<class BoundingVolumeClass>
//void BVHNode<BoundingVolumeClass>::recalculateBoundingVolume(bool recurse)
//{
//	if(isLeaf())
//	{
//		return;
//	}
//
//	m_volume = BoundingVolumeClass(m_children[0]->m_volume, m_children[1]->m_volume);
//
//	if(m_parent)
//	{
//		m_parent->recalculateBoundingVolume(true);
//	}
//}
//
//BoundingSphere_::BoundingSphere_(const Vector3& centre, float radius)
//{
//	m_centre = centre;
//	m_radius = radius;
//}
//
//BoundingSphere_::BoundingSphere_(const BoundingSphere_& one, const BoundingSphere_& two)
//{
//	Vector3 centreOffset = two.m_centre - one.m_centre;
//	float distance = centreOffset.length() * centreOffset.length();
//	float radiusDifference = two.m_radius - one.m_radius;
//
//	if(radiusDifference * radiusDifference >= distance)
//	{
//		if(one.m_radius > two.m_radius)
//		{
//			m_centre = one.m_centre;
//			m_radius = one.m_radius;
//		}
//		else
//		{
//			m_centre = two.m_centre;
//			m_radius = two.m_radius;
//		}
//	}
//	else
//	{
//		distance = sqrtf(distance);
//		m_radius = (distance + one.m_radius + two.m_radius) * 0.5f;
//
//		m_centre = one.m_centre;
//
//		if(distance > 0)
//		{
//			m_centre += centreOffset * ((m_radius - one.m_radius) / distance);
//		}
//	}
//}
//
//bool BoundingSphere_::overlaps(const BoundingSphere_* other) const
//{
//	float distanceSquared = (m_centre - other->m_centre).length() * (m_centre - other->m_centre).length();
//	return distanceSquared < (m_radius + other->m_radius) * (m_radius + other->m_radius);
//}
//
//float BoundingSphere_::getGrowth(const BoundingSphere_& other) const
//{
//	BoundingSphere_ newSphere(*this, other);
//	return newSphere.m_radius * newSphere.m_radius  - m_radius * m_radius;
//}
//
//float BoundingSphere_::getSize() const
//{
//	return ((float)1.333333) * PI * m_radius * m_radius * m_radius;
//}