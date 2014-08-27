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

#endif