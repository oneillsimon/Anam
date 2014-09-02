#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <map>
#include <vector>

#include "../Core/ReferenceCounter.h"
#include "Shader.h"
#include "Vertex.h"

#define MESH_DRAW_WIREFRAMES 0

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

	void addToTexCoords(int index, const Vector2& value);

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
	Shader m_wireFrameShader;

	MeshData(MeshData& other) {}
	void operator =(MeshData& meshData) {}

public:
	MeshData(const IndexedModel& model);
	virtual ~MeshData();
	
	void updateTextureBuffer(const IndexedModel& model);

	void draw(int primitive) const;
	void drawWireFrame() const;

	Shader getWireFrameShader() const;
};

class Mesh
{
protected:
	static std::map<std::string, MeshData*> resourceMap;
	std::string m_fileName;
	MeshData* m_meshData;
	
public:
	Mesh();
	Mesh(const std::string& fileName);
	Mesh(const std::string& meshName, const IndexedModel& model);
	Mesh(const Mesh& mesh);
	virtual ~Mesh();

	void draw(int primitive = GL_TRIANGLES) const;
	void drawWireFrame() const;

	Shader getWireFrameShader() const;
};

#endif