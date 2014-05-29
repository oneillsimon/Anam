#include "Vertex.h"

Vertex::Vertex(Vector3 position, Vector2 texCoord, Vector3 normal)
{
	this->position = position;
	this->texCoord = texCoord;
	this->normal = normal;
}

Vector3 Vertex::getPosition()
{
	return position;
}

Vector2 Vertex::getTexCoord()
{
	return texCoord;
}

Vector3 Vertex::getNormal()
{
	return normal;
}

void Vertex::setPosition(Vector3 position)
{
	this->position = position;
}

void Vertex::setTexCoord(Vector2 texCoord)
{
	this->texCoord = texCoord;
}

void Vertex::setNormal(Vector3 normal)
{
	this->normal = normal;
}