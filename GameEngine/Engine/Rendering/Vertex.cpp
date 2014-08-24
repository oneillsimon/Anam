#include "Vertex.h"

Vertex::Vertex(Vector3 position, Vector2 texCoord, Vector3 normal)
{
	m_position = position;
	m_texCoord = texCoord;
	m_normal = normal;
	m_tangent = Vector3();
}

Vertex::Vertex(Vector3 position, Vector2 texCoord, Vector3 normal, Vector3 tangent)
{
	m_position = position;
	m_texCoord = texCoord;
	m_normal = normal;
	m_tangent = tangent;
}

Vector3 Vertex::getPosition() const
{
	return m_position;
}

Vector2 Vertex::getTexCoord() const
{
	return m_texCoord;
}

Vector3 Vertex::getNormal() const
{
	return m_normal;
}

Vector3 Vertex::getTangent() const
{
	return m_tangent;
}

void Vertex::setPosition(const Vector3& position)
{
	m_position = position;
}

void Vertex::setTexCoord(const Vector2& texCoord)
{
	m_texCoord = texCoord;
}

void Vertex::setNormal(const Vector3& normal)
{
	m_normal = normal;
}

void Vertex::setTangent(const Vector3& tangent)
{
	m_tangent = tangent;
}