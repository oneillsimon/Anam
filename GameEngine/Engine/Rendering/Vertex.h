#pragma once

#include "../Core/3DMath.h"

class Vertex
{
private:
	Vector3 position;
	Vector2 texCoord;
	Vector3 normal;

public:
	Vertex(Vector3 position, Vector2 texCoord, Vector3 normal);

	Vector3 getPosition();
	Vector2 getTexCoord();
	Vector3 getNormal();

	void setPosition(Vector3 position);
	void setTexCoord(Vector2 texCoord);
	void setNormal(Vector3 normal);
};