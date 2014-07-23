#ifndef LINEMESH_H
#define LINEMESH_H

#include "../Core/GameObject.h"
#include "../Core/Math3D.h"
#include "Mesh.h"

class LineMesh : public Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<int> indices;

public:
	LineMesh(GameObject* obj0, GameObject* obj1, Colour* colour);
	LineMesh(const Vector3& m_position0, const Vector3& m_position1, Colour* colour);

	virtual void draw();
};

#endif