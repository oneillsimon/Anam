#ifndef VERTEX_H
#define VERTEX_H

#include "../Core/Math3D.h"

class Vertex
{
private:
	Vector3 m_position;
	Vector2 m_texCoord;
	Vector3 m_normal;
	Vector3 m_tangent;

public:
	Vertex(Vector3 position, Vector2 texCoord, Vector3 normal);
	Vertex(Vector3 position, Vector2 texCoord, Vector3 normal, Vector3 tangent);

	Vector3 getPosition() const;
	Vector2 getTexCoord() const;
	Vector3 getNormal() const;
	Vector3 getTangent() const;

	void setPosition(const Vector3& position);
	void setTexCoord(const Vector2& texCoord);
	void setNormal(const Vector3& normal);
	void setTangent(const Vector3& tangent);
};

#endif