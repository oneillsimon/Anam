#ifndef MESH_H
#define MESH_H

#include <string>
#include <map>
#include "ReferenceCounter.h"
#include "Vertex.h"

class MeshData : public ReferenceCounter
{
private:
	unsigned int vbo;
	unsigned int ibo;
	int size;

	void operator =(MeshData& meshData) {}

public:
	MeshData(int indexSize);
	~MeshData();

	int getVbo();
	int getIbo();
	int getSize();
};

class Mesh
{
protected:
	static std::map<std::string, MeshData*> resourceMap;

	std::string m_fileName;
	MeshData* m_meshData;

	void calculateNormals(Vertex* vertices, int vertSize, int* indices, int indexSize);
	void initMesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals);

	void operator =(Mesh& mesh) {}
	
public:
	Mesh();
	Mesh(std::string fileName);
	Mesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals);
	~Mesh();

	virtual void draw();
	void draw(int gl_primitives);
};

#endif