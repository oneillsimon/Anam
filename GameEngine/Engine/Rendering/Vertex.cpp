#include "Vertex.h"

Vertex::Vertex(Vector3 position, Vector2 texCoord, Vector3 normal)
{
	m_position = position;
	m_texCoord = texCoord;
	m_normal = normal;
	m_tangent = Vector3::ZERO;
}

Vertex::Vertex(Vector3 position, Vector2 texCoord, Vector3 normal, Vector3 tangent)
{
	m_position = position;
	m_texCoord = texCoord;
	m_normal = normal;
	m_tangent = tangent;
}

Vector3 Vertex::getPosition()
{
	return m_position;
}

Vector2 Vertex::getTexCoord()
{
	return m_texCoord;
}

Vector3 Vertex::getNormal()
{
	return m_normal;
}

Vector3 Vertex::getTangent()
{
	return m_tangent;
}

void Vertex::setPosition(Vector3 position)
{
	m_position = position;
}

void Vertex::setTexCoord(Vector2 texCoord)
{
	m_texCoord = texCoord;
}

void Vertex::setNormal(Vector3 normal)
{
	m_normal = normal;
}

void Vertex::setTangent(Vector3 tangent)
{
	m_tangent = tangent;
}