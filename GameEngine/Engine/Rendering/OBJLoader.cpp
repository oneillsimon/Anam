#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include "OBJLoader.h"

static bool compareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b);
static inline unsigned int findNextChar(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int parseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end);
static inline float parseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> splitString(const std::string &s, char delim);

static Vector2 parseOBJVec2(const std::string& line);
static Vector3 parseOBJVec3(const std::string& line);
static OBJIndex parseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals);

OBJModel::OBJModel(const std::string& fileName)
{
	hasUVs = false;
	hasNormals = false;
    std::ifstream file;
    file.open(fileName.c_str());

    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
        
            size_t lineLength = line.length();
            
            if(lineLength < 2)
			{
                continue;
			}
            
            const char* lineCStr = line.c_str();
            
            switch(lineCStr[0])
            {
                case 'v':
                    if(lineCStr[1] == 't')
					{
						this->uvs.push_back(parseOBJVec2(line)); 
					}
                    else if(lineCStr[1] == 'n')
					{
                        this->normals.push_back(parseOBJVec3(line));
					}
                    else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
					{
                        this->vertices.push_back(parseOBJVec3(line));
					}
                break;
                case 'f':
                    createOBJFace(line);
                break;
                default:
					break;
            };
        }
    }
    else
    {
        std::cerr << "Unable to load mesh: " << fileName << std::endl;
    }
}

bool OBJIndex::operator <(const OBJIndex& o) const
{
	return vertexIndex < o.vertexIndex;
}

void IndexedModel::calculateNormals()
{
    for(unsigned int i = 0; i < indices.size(); i += 3)
    {
        unsigned int i0 = indices[i];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        Vector3 v1 = positions[i1] - positions[i0];
        Vector3 v2 = positions[i2] - positions[i0];
        
        Vector3 normal = v1.cross(v2).normalised();
            
        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }
    
    for(unsigned int i = 0; i < normals.size(); i++)
	{
        normals[i] = normals[i].normalised();
	}
}

IndexedModel OBJModel::toIndexedModel()
{
    IndexedModel result;
    IndexedModel normalModel;
    
    size_t numIndices = objIndices.size();
    
    std::vector<OBJIndex*> indexLookup;
    
    for(unsigned int i = 0; i < numIndices; i++)
	{
        indexLookup.push_back(&objIndices[i]);
	}
    
    std::sort(indexLookup.begin(), indexLookup.end(), compareOBJIndexPtr);
    
    std::map<OBJIndex, unsigned int> normalModelIndexMap;
    std::map<unsigned int, unsigned int> indexMap;
    
    for(unsigned int i = 0; i < numIndices; i++)
    {
        OBJIndex* currentIndex = &objIndices[i];
        
        Vector3 currentPosition = vertices[currentIndex->vertexIndex];
        Vector2 currentTexCoord;
        Vector3 currentNormal;
        
        if(hasUVs)
		{
            currentTexCoord = uvs[currentIndex->uvIndex];
		}
        else
		{
			currentTexCoord = Vector2::ZERO;
		}
            
        if(hasNormals)
		{
            currentNormal = normals[currentIndex->normalIndex];
		}
        else
		{
			currentNormal = Vector3::ZERO;
		}
        
        unsigned int normalModelIndex;
        unsigned int resultModelIndex;
        
        std::map<OBJIndex, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);

        if(it == normalModelIndexMap.end())
        {
            normalModelIndex = (unsigned int)normalModel.positions.size();
        
            normalModelIndexMap.insert(std::pair<OBJIndex, unsigned int>(*currentIndex, normalModelIndex));
            normalModel.positions.push_back(currentPosition);
            normalModel.texCoords.push_back(currentTexCoord);
            normalModel.normals.push_back(currentNormal);
        }
        else
		{
            normalModelIndex = it->second;
		}

		unsigned int previousVertexLocation = findLastVertexIndex(indexLookup, currentIndex, result);
        
        if(previousVertexLocation == (unsigned int)-1)
        {
            resultModelIndex = (unsigned int)result.positions.size();
        
            result.positions.push_back(currentPosition);
            result.texCoords.push_back(currentTexCoord);
            result.normals.push_back(currentNormal);
        }
        else
		{
            resultModelIndex = previousVertexLocation;
		}
        
        normalModel.indices.push_back(normalModelIndex);
        result.indices.push_back(resultModelIndex);
        indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
    }
    
    if(!hasNormals)
    {
        normalModel.calculateNormals();
        
        for(unsigned int i = 0; i < result.positions.size(); i++)
		{
            result.normals[i] = normalModel.normals[indexMap[i]];
		}
    }
    
    return result;
}

unsigned int OBJModel::findLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result) const
{
    unsigned int start = 0;
    unsigned int end = (unsigned int)indexLookup.size();
    unsigned int current = (end - start) / 2 + start;
    unsigned int previous = start;
    
    while(current != previous)
    {
        OBJIndex* testIndex = indexLookup[current];
        
        if(testIndex->vertexIndex == currentIndex->vertexIndex)
        {
            unsigned int countStart = current;
        
            for(unsigned int i = 0; i < current; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current - i];
                
                if(possibleIndex == currentIndex)
				{
                    continue;
				}
                    
                if(possibleIndex->vertexIndex != currentIndex->vertexIndex)
				{
                    break;
				}
                    
                countStart--;
            }
            
            for(unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current + i];
                
                if(possibleIndex == currentIndex)
				{
                    continue;
				}
                    
                if(possibleIndex->vertexIndex != currentIndex->vertexIndex)
				{
                    break;
				}
                else if((!hasUVs || possibleIndex->uvIndex == currentIndex->uvIndex) &&
						(!hasNormals || possibleIndex->normalIndex == currentIndex->normalIndex))
                {
                    Vector3 currentPosition = vertices[currentIndex->vertexIndex];
                    Vector2 currentTexCoord;
                    Vector3 currentNormal;
                    
                    if(hasUVs)
					{
                        currentTexCoord = uvs[currentIndex->uvIndex];
					}
                    else
					{
                        currentTexCoord = Vector2(0,0);
					}
                        
                    if(hasNormals)
					{
                        currentNormal = normals[currentIndex->normalIndex];
					}
                    else
					{
                        currentNormal = Vector3(0,0,0);
					}
                    
                    for(unsigned int j = 0; j < result.positions.size(); j++)
                    {
                        if(currentPosition == result.positions[j] &&
						   ((!hasUVs || currentTexCoord == result.texCoords[j]) &&
						   (!hasNormals || currentNormal == result.normals[j])))
                        {
                            return j;
                        }
                    }
                }
            }
        
            return (unsigned int)-1;
        }
        else
        {
            if(testIndex->vertexIndex < currentIndex->vertexIndex)
			{
                start = current;
			}
            else
			{
                end = current;
			}
        }
    
        previous = current;
        current = (end - start) / 2 + start;
    }
    
    return (unsigned int)-1;
}

void OBJModel::createOBJFace(const std::string& line)
{
    std::vector<std::string> tokens = splitString(line, ' ');

	for(size_t i = 0; i < tokens.size() - 3; i++)
	{
		this->objIndices.push_back(parseOBJIndex(tokens[1], &this->hasUVs, &this->hasNormals));
		this->objIndices.push_back(parseOBJIndex(tokens[2 + i], &this->hasUVs, &this->hasNormals));
		this->objIndices.push_back(parseOBJIndex(tokens[3 + i], &this->hasUVs, &this->hasNormals));
	}
}

OBJIndex parseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals)
{
    unsigned int tokenLength = (unsigned int)token.length();
    const char* tokenString = token.c_str();
    
    unsigned int vertIndexStart = 0;
    unsigned int vertIndexEnd = findNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    OBJIndex result;
    result.vertexIndex = parseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    result.uvIndex = 0;
    result.normalIndex = 0;
    
    if(vertIndexEnd >= tokenLength)
	{
        return result;
	}
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = findNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.uvIndex = parseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasUVs = true;
    
    if(vertIndexEnd >= tokenLength)
	{
        return result;
	}
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = findNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.normalIndex = parseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasNormals = true;
    
    return result;
}

Vector3 parseOBJVec3(const std::string& line) 
{
    unsigned int tokenLength = (unsigned int)line.length();
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 2;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = findNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = parseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = findNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = parseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = findNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float z = parseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return Vector3(x, y, z);
}

Vector2 parseOBJVec2(const std::string& line)
{
    unsigned int tokenLength = (unsigned int)line.length();
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 3;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
		{
            break;
		}
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = findNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = parseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = findNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = parseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return Vector2(x,y);
}

static bool compareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b)
{
    return a->vertexIndex < b->vertexIndex;
}

static inline unsigned int findNextChar(unsigned int start, const char* str, unsigned int length, char token)
{
    unsigned int result = start;

    while(result < length)
    {
        result++;

        if(str[result] == token)
		{
            break;
		}
    }
    
    return result;
}

static inline unsigned int parseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
    return (unsigned int)(atoi(token.substr(start, end - start).c_str()) - 1);
}

static inline float parseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
    return (float)atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> splitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;
        
    const char* cstr = s.c_str();
    unsigned int strLength = (unsigned int)s.length();
    unsigned int start = 0;
    unsigned int end = 0;
        
    while(end <= strLength)
    {
        while(end <= strLength)
        {
            if(cstr[end] == delim)
			{
                break;
			}

            end++;
        }
            
        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }

    return elems;
}