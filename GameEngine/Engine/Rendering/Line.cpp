#include <vector>
#include "GL/glew.h"
#include "Vertex.h"
#include "Line.h"

Line::Line(const Vector3& point0, const Vector3& point1)
{
	this->point0 = point0;
	this->point1 = point1;

	std::vector<Vertex> vertices;
	std::vector<int> indices;

	Vertex v1(Vector3(point0.getX(), point0.getY(), point0.getZ()), Vector2(0, 1), Vector3::ZERO);
	Vertex v2(Vector3(point1.getX(), point1.getY(), point1.getZ()), Vector2(0, 1), Vector3::ZERO);
	vertices.push_back(v1);
	vertices.push_back(v2);

	indices.push_back(0);
	indices.push_back(1);

	initMesh(&vertices[0], vertices.size(), &indices[0], indices.size(), false);
}

void Line::draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->getVbo());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3));
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3) + sizeof(Vector2)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshData->getIbo());
	glDrawElements(GL_LINES, m_meshData->getSize(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);
}