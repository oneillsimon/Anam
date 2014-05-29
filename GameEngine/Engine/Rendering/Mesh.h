#pragma once

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
private:
	static std::map<std::string, MeshData*> resourceMap;

	std::string m_fileName;
	MeshData* m_meshData;

	void calculateNormals(Vertex* vertices, int vertSize, int* indices, int indexSize);
	void initMesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals);

	void operator =(Mesh& mesh) {}
	
public:
	Mesh(std::string fileName);
	Mesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals);
	~Mesh();

	void draw();
};