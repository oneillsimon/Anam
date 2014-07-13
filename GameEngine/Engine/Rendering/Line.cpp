#include <vector>
#include "GL/glew.h"
#include "Vertex.h"
#include "Mesh.h"
#include "../Rendering/Shader.h"
#include "Line.h"

Line::Line(const Vector3& position0, const Vector3& position1, Colour* colour)
{
	this->point0 = position0;
	this->point1 = position1;
	m_colour = colour;
	m_material = new Material(new Texture("test.png"), 1, 8);
	
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	Vertex v1(Vector3(point0.getX(), point0.getY(), point0.getZ()), Vector2(0, 1), Vector3::ZERO);
	Vertex v2(Vector3(point1.getX(), point1.getY(), point1.getZ()), Vector2(0, 1), Vector3::ZERO);

	vertices.push_back(v1);
	vertices.push_back(v2);

	indices.push_back(0);
	indices.push_back(1);

	m_mesh = new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size(), false);
	//m_parent->addComponent(new MeshRenderer(new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size(), false), colour));
	//m_meshRenderer->setParent(m_parent);
}

void Line::update(float delta)
{
}

void Line::render(Shader* shader, RenderingEngine* renderingEngine)
{
	shader->bind();
	shader->updateUniforms(getTransform(), *m_material, *m_colour, renderingEngine);
	m_mesh->draw(GL_LINES);
}