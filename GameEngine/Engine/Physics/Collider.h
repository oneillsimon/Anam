#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Core/ReferenceCounter.h"
#include "../Rendering/Mesh.h"
#include "IntersectionData.h"

class Collider : public ReferenceCounter
{
private:
	int m_type;

protected:
	Mesh m_mesh;
	Vector3 m_scale;

public:
	enum
	{
		TYPE_SPHERE,
		TYPE_AABB,
		TYPE_SIZE
	};

	Collider(int type, const std::string& meshName = "");

	IntersectionData intersect(const Collider& collider) const;
	virtual void transform(const Vector3& translation) {};

	int getType() const;

	virtual Vector3 getCentre() const;
	Mesh getMesh() const;
	Vector3 getScale() const;
};

class AABB : public Collider
{
private:
	const Vector3 m_minExtents;
	const Vector3 m_maxExtents;
public:
	AABB(const Vector3& minExtends, const Vector3& maxExtents);

	IntersectionData intersectAABB(const AABB& other) const;

	const Vector3& getMinExtents() const;
	const Vector3& getMaxExtents() const;
};

class BoundingSphere : public Collider
{
private:
	Vector3 m_center;
	float m_radius;

public:
	BoundingSphere(Vector3 center, float radius);

	IntersectionData intersectBoundingSphere(const BoundingSphere& other);
	virtual void transform(const Vector3& translation);

	virtual Vector3 getCentre() const;
	const float getRadius() const;

	void setCenter(Vector3 center);
	void setRadius(float radius);
};

class Plane
{
private:
	const Vector3 m_normal;
	const float m_distance;

public:
	Plane(const Vector3& normal, float distance);

	Plane normalised() const;

	IntersectionData intersectSphere(const BoundingSphere& sphere) const;

	const Vector3& getNormal() const;
	float getDistance() const;
};

#endif