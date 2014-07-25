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

	Vector3 getPosition();
	Vector2 getTexCoord();
	Vector3 getNormal();
	Vector3 getTangent();

	void setPosition(Vector3 position);
	void setTexCoord(Vector2 texCoord);
	void setNormal(Vector3 normal);
	void setTangent(Vector3 tangent);
};

#endif