#include <vector>
#include "GL/glew.h"
#include "Vertex.h"
#include "Mesh.h"
#include "../Components/MeshRenderer.h"
#include "Line.h"

Line::Line(const Vector3& position0, const Vector3& position1, Colour* colour)
{
	this->point0 = position0;
	this->point1 = position1;
	
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	Vertex v1(Vector3(point0.getX(), point0.getY(), point0.getZ()), Vector2(0, 1), Vector3::ZERO);
	Vertex v2(Vector3(point1.getX(), point1.getY(), point1.getZ()), Vector2(0, 1), Vector3::ZERO);

	vertices.push_back(v1);
	vertices.push_back(v2);

	indices.push_back(0);
	indices.push_back(1);

	m_mesh = new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size(), false);
	//m_meshRenderer = new MeshRenderer(m_mesh, colour);
}

void Line::update(float delta)
{
}

void Line::render(Shader* shader, RenderingEngine* renderingEngine)
{
	//m_meshRenderer->render(shader, renderingEngine);
}