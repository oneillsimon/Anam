#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <string>
#include "../Core/Math3D.h"

class OBJIndex
{
private:
	unsigned int vertexIndex;
	unsigned int uvIndex;
	unsigned int normalIndex;

public:
	unsigned int getVertexIndex() const;
	unsigned int getUVIndex() const;
	unsigned int getNormalIndex() const;

	void setVertexIndex(unsigned int vertexIndex);
	void setUVIndex(unsigned int uvIndex);
	void setNormalIndex(unsigned int normalIndex);

	bool operator <(const OBJIndex& o) const;
};

class IndexedModel
{
private:
	std::vector<Vector3> positions;
	std::vector<Vector2> texCoords;
	std::vector<Vector3> normals;
	std::vector<unsigned int> indices;

public:
	void calculateNormals();

	std::vector<Vector3> getPositions() const;
	std::vector<Vector2> getTexCoords() const;
	std::vector<Vector3> getNormals() const;
	std::vector<unsigned int> getIndices();
};

class OBJModel
{
private:
	std::vector<OBJIndex> objIndices;
	std::vector<Vector3> vertices;
	std::vector<Vector2> uvs;
	std::vector<Vector3> normals;
	bool hasUVs;
	bool hasNormals;

	unsigned int findLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result) const;
	void createOBJFace(const std::string& line);

public:
	OBJModel(const std::string& filename);

	IndexedModel toIndexedModel();
};

#endif