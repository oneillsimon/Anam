#include "GL/glew.h"
#include "Colour.h"
#include "LineMesh.h"

LineMesh::LineMesh(GameObject* obj0, GameObject* obj1, Colour* colour)
{
	vertices.push_back(Vertex(Vector3(obj0->getTransform().getPosition().getX(),
									  obj0->getTransform().getPosition().getY(),
									  obj0->getTransform().getPosition().getZ()),
									  Vector2(0, 1),
									  Vector3::ZERO));
	vertices.push_back(Vertex(Vector3(obj1->getTransform().getPosition().getX(),
									  obj1->getTransform().getPosition().getY(),
									  obj1->getTransform().getPosition().getZ()),
									  Vector2(0, 1),
									  Vector3::ZERO));
	indices.push_back(0);
	indices.push_back(1);

	initMesh(&vertices[0], vertices.size(), &indices[0], indices.size(), false);
}

LineMesh::LineMesh(const Vector3& position0, const Vector3& position1, Colour* colour)
{
	vertices.push_back(Vertex(Vector3(position0.getX(), position0.getY(), position0.getZ()), Vector2(0, 1), Vector3::ZERO));
	vertices.push_back(Vertex(Vector3(position1.getX(), position1.getY(), position1.getZ()), Vector2(0, 1), Vector3::ZERO));

	indices.push_back(0);
	indices.push_back(1);

	initMesh(&vertices[0], vertices.size(), &indices[0], indices.size(), false);
}

void LineMesh::draw()
{
	Mesh::draw(GL_LINES);
}