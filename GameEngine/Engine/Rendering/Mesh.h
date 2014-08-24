#ifndef MESH_H
#define MESH_H

#include <string>
#include <map>
#include <vector>
#include <GL/glew.h>
#include "../Core/ReferenceCounter.h"
#include "Vertex.h"
#include "../Core/Math3D.h"

class IndexedModel
{
private:
	std::vector<unsigned int> m_indices;
	std::vector<Vector3> m_positions;
	std::vector<Vector2> m_texCoords;
	std::vector<Vector3> m_normals;
	std::vector<Vector3> m_tangents;

public:
	IndexedModel() {}
	IndexedModel(const std::vector<unsigned int> indices,
				 const std::vector<Vector3> positions,
				 const std::vector<Vector2> texCoords,
				 const std::vector<Vector3> normals,
				 const std::vector<Vector3> tangents);

	bool isValid() const;
	void calculateNormals();
	void calculateTangents();

	IndexedModel finalize();

	void addVertex(const Vector3& v);
	void addVertex(int x, int y, int z);

	void addTexCoord(const Vector2& v);
	void addTexCoord(int x, int y);

	void addNormal(const Vector3& v);
	void addNormal(int x, int y, int z);

	void addTangent(const Vector3& v);
	void addTangent(int x, int y, int z);

	void addFace(unsigned int vertexIndex0, unsigned int vertexIndex1, unsigned int vertexIndex2);

	const std::vector<unsigned int>& getIndices() const;
	const std::vector<Vector3>& getPositions() const;
	const std::vector<Vector2>& getTexCoords() const;
	const std::vector<Vector3>& getNormals() const;
	const std::vector<Vector3>& getTangents() const;
};

class MeshData : public ReferenceCounter
{
private:
	enum
	{
		POSITION_VB, TEXTCOORD_VB, NORMAL_VB, TANGENT_VB, INDEX_VB, NUM_BUFFERS
	};

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	int m_drawCount;

	MeshData(MeshData& other) {}
	void operator =(MeshData& meshData) {}

public:
	MeshData(const IndexedModel& model);
	virtual ~MeshData();
	
	void draw() const;
};

class Mesh
{
protected:
	static std::map<std::string, MeshData*> resourceMap;

	std::string m_fileName;
	MeshData* m_meshData;
	
public:
	Mesh(const std::string& fileName = "cube.obj");
	Mesh(const std::string& meshName, const IndexedModel& model);
	Mesh(const Mesh& mesh);
	virtual ~Mesh();

	void draw() const;
	
	void operator =(Mesh& mesh) {}
};

#endif