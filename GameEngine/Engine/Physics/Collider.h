#ifndef COLLIDER_H
#define COLLIDER_H

#include "../Core/GameComponent.h"
#include "../Core/ReferenceCounter.h"
#include "../Rendering/Mesh.h"
#include "IntersectionData.h"

class Collider : public ReferenceCounter, public GameComponent
{
private:
	int m_type;
	mutable bool m_isColliding;

protected:
	Mesh m_mesh;
	Colour c;
	Vector3 m_scale;

public:
	enum
	{
		TYPE_SPHERE,
		TYPE_AABB,
		TYPE_PLANE
	};

	Collider(int type, const std::string& meshName);

	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;

	IntersectionData intersect(const Collider& collider) const;
	virtual void transform(const Vector3& translation) {};

	int getType() const;
	bool getIsColliding() const;

	Vector3 getCentre() const;
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
	float m_radius;

public:
	BoundingSphere(float radius);

	IntersectionData intersectBoundingSphere(const BoundingSphere& other);
	IntersectionData intersectAABB(const AABB& other);
	virtual void transform(const Vector3& translation);

	const float getRadius() const;

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