#ifndef OBJ_LOADER_H_INCLUDED
#define OBJ_LOADER_H_INCLUDED

#include <vector>
#include <string>
#include "../Core/Math3D.h"

struct OBJIndex
{
    unsigned int vertexIndex;
    unsigned int uvIndex;
    unsigned int normalIndex;
    
    bool operator <(const OBJIndex& o) const;
};

class IndexedModel
{
public:
    std::vector<Vector3> positions;
    std::vector<Vector2> texCoords;
    std::vector<Vector3> normals;
    std::vector<unsigned int> indices;
    
    void calculateNormals();
};

class OBJModel
{
public:
    std::vector<OBJIndex> objIndices;
    std::vector<Vector3> vertices;
    std::vector<Vector2> uvs;
    std::vector<Vector3> normals;
    bool hasUVs;
    bool hasNormals;
    
    OBJModel(const std::string& fileName);
    
    IndexedModel toIndexedModel();
private:
    unsigned int findLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result) const;
    void createOBJFace(const std::string& line);
};

#endif