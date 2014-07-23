#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include "OBJLoader.h"

static bool compareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b);
static unsigned int findNextChar(unsigned int start, const char* str, unsigned int length, char token);
static unsigned int parseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end);
static float parseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end);
static std::vector<std::string> splitString(const std::string& s, char delim);

static Vector2 parseOBJVector2(const std::string& line);
static Vector3 parseOBJVector3(const std::string& line);
static OBJIndex parseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals);

OBJModel::OBJModel(const std::string& filename)
{
	hasUVs = false;
	hasNormals = false;
	std::ifstream file;
	file.open(filename.c_str());

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
					this->uvs.push_back(parseOBJVector2(line));
				}
				else if(lineCStr[1] == 'n')
				{
					this->normals.push_back(parseOBJVector3(line));
				}
				else if(lineCStr[1] == '\t' || lineCStr[1] == ' ')
				{
					this->vertices.push_back(parseOBJVector3(line));
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
		std::cerr << "Unable to load mesh: " << filename << std::endl;
	}
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

	std::vector<OBJIndex*> indexLookUp;

	for(unsigned int i = 0; i < numIndices; i++)
	{
		indexLookUp.push_back(&objIndices[i]);
	}

	std::sort(indexLookUp.begin(), indexLookUp.end(), compareOBJIndexPtr);

	std::map<OBJIndex, unsigned int> normalModelIndexMap;
	std::map<unsigned int, unsigned int> indexMap;

	for(unsigned int i = 0; i < numIndices; i++)
	{
		OBJIndex* currentIndex = &objIndices[i];

		Vector3 currentPosition = vertices[currentIndex->getVertexIndex()];
		Vector2 currentTexCoord;
		Vector3 currentNormal;

		if(hasUVs)
		{
			currentTexCoord = uvs[currentIndex->getUVIndex()];
		}
		else
		{
			currentTexCoord = Vector2::ZERO;
		}

		if(hasNormals)
		{
			currentNormal = normals[currentIndex->getNormalIndex()];
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
			normalModelIndex = (unsigned int)normalModel.getPositions().size();

			normalModelIndexMap.insert(std::pair<OBJIndex, unsigned int>(*currentIndex, normalModelIndex));
			normalModel.getPositions().push_back(currentPosition);
			normalModel.getTexCoords().push_back(currentTexCoord);
			normalModel.getNormals().push_back(currentNormal);
		}
		else
		{
			normalModelIndex = it->second;
		}

		unsigned int previousVertexLocation = findLastVertexIndex(indexLookUp, currentIndex, result);

		if(previousVertexLocation == (unsigned int)-1)
		{
			resultModelIndex = (unsigned int)result.getPositions().size();

			result.getPositions().push_back(currentPosition);
			result.getTexCoords().push_back(currentTexCoord);
			result.getNormals().push_back(currentNormal);
		}
		else
		{
			resultModelIndex = previousVertexLocation;
		}

		normalModel.getIndices().push_back(normalModelIndex);
		result.getIndices().push_back(resultModelIndex);
		indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
	}

	if(!hasNormals)
	{
		normalModel.calculateNormals();

		for(unsigned int i = 0; i < result.getPositions().size(); i++)
		{
			result.getNormals()[i] = normalModel.getNormals()[indexMap[i]];
		}
	}

	return result;
}

unsigned int OBJModel::findLastVertexIndex(const std::vector<OBJIndex*>& indexLookUp, const OBJIndex* currentIndex, const IndexedModel& result) const
{
	unsigned int start = 0;
	unsigned int end = (unsigned int)indexLookUp.size();
	unsigned int current = (end - start) / 2 + start;
	unsigned int previous = start;

	while(current != previous)
	{
		OBJIndex* testIndex = indexLookUp[current];

		if(testIndex->getVertexIndex() == currentIndex->getVertexIndex())
		{
			unsigned int countStart = current;

			for(unsigned int i = 0; i < current; i++)
			{
				OBJIndex* possibleIndex = indexLookUp[current - i];

				if(possibleIndex == currentIndex)
				{
					continue;
				}

				if(possibleIndex->getVertexIndex() != currentIndex->getVertexIndex())
				{
					break;
				}

				countStart--;
			}

			for(unsigned int i = countStart; i < indexLookUp.size(); i++)
			{
				OBJIndex* possibleIndex = indexLookUp[current + i];

				if(possibleIndex == currentIndex)
				{
					continue;
				}

				if(possibleIndex->getVertexIndex() != currentIndex->getVertexIndex())
				{
					break;
				}
				else if((!hasUVs || possibleIndex->getUVIndex() == currentIndex->getUVIndex()) &&
						(!hasNormals || possibleIndex->getNormalIndex() == currentIndex->getNormalIndex()))
				{
					Vector3 currentPosition = vertices[currentIndex->getVertexIndex()];
					Vector2 currentTexCoord;
					Vector3 currentNormal;

					if(hasUVs)
					{
						currentTexCoord = uvs[currentIndex->getUVIndex()];
					}
					else
					{
						currentTexCoord = Vector2::ZERO;
					}

					if(hasNormals)
					{
						currentNormal = normals[currentIndex->getUVIndex()];
					}
					else
					{
						currentNormal = Vector3::ZERO;
					}

					for(unsigned int j = 0; j < result.getPositions().size(); j++)
					{
						if(currentPosition == result.getPositions()[j] &&
						  ((!hasUVs || currentTexCoord == result.getTexCoords()[j]) &&
						  (!hasNormals || currentNormal == result.getNormals()[j])))
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
			if(testIndex->getVertexIndex() < currentIndex->getVertexIndex())
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
	result.setVertexIndex(parseOBJIndexValue(token, vertIndexStart, vertIndexEnd));
	result.setUVIndex(0);
	result.setNormalIndex(0);

	if(vertIndexEnd >= tokenLength)
	{
		return result;
	}

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = findNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.setUVIndex(parseOBJIndexValue(token, vertIndexStart, vertIndexEnd));
	*hasUVs = true;

	if(vertIndexEnd >= tokenLength)
	{
		return result;
	}

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = findNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.setNormalIndex(parseOBJIndexValue(token, vertIndexStart, vertIndexEnd));
	*hasNormals = true;

	return result;
}

Vector3 parseOBJVector3(const std::string& line)
{
	unsigned int tokenLength = (unsigned int)line.length();
	const char* toeknString = line.c_str();

	unsigned int vertIndexStart = 2;

	while(vertIndexStart < tokenLength)
	{
		if(toeknString[vertIndexStart] != ' ')
		{
			break;
		}

		vertIndexStart++;
	}

	unsigned int vertIndexEnd = findNextChar(vertIndexStart, toeknString, tokenLength, ' ');

	float x = parseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = findNextChar(vertIndexStart, toeknString, tokenLength, ' ');

	float y = parseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = findNextChar(vertIndexStart, toeknString, tokenLength, ' ');

	float z = parseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return Vector3(x, y, z);
}

Vector2 parseOBJVector2(const std::string& line)
{
	unsigned int tokenLength = (unsigned int)line.length();
	const char* toeknString = line.c_str();

	unsigned int vertIndexStart = 3;

	while(vertIndexStart < tokenLength)
	{
		if(toeknString[vertIndexStart] != ' ')
		{
			break;
		}

		vertIndexStart++;
	}

	unsigned int vertIndexEnd = findNextChar(vertIndexStart, toeknString, tokenLength, ' ');

	float x = parseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = findNextChar(vertIndexStart, toeknString, tokenLength, ' ');

	float y = parseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return Vector2(x, y);
}

static bool compareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b)
{
	return a->getVertexIndex() < b->getVertexIndex();
}

static unsigned int findNextChar(unsigned int start, const char* str, unsigned int length, char token)
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

static unsigned int parseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
	return (unsigned int)(atoi(token.substr(start, end - start).c_str()) - 1);
}

static float parseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
	return (float)atof(token.substr(start, end - start).c_str());
}

static std::vector<std::string> splitString(const std::string& s, char delim)
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

unsigned int OBJIndex::getVertexIndex() const
{
	return vertexIndex;
}

unsigned int OBJIndex::getUVIndex() const
{
	return uvIndex;
}

unsigned int OBJIndex::getNormalIndex() const
{
	return normalIndex;
}

void OBJIndex::setVertexIndex(unsigned int vertexIndex)
{
	this->vertexIndex = vertexIndex;
}

void OBJIndex::setUVIndex(unsigned int uvIndex)
{
	this->uvIndex = uvIndex;
}

void OBJIndex::setNormalIndex(unsigned int normalIndex)
{
	this->normalIndex = normalIndex;
}

bool OBJIndex::operator <(const OBJIndex& o) const
{
	return vertexIndex < o.getVertexIndex();
}

std::vector<Vector3> IndexedModel::getPositions() const
{
	return positions;
}

std::vector<Vector2> IndexedModel::getTexCoords() const
{
	return texCoords;
}

std::vector<Vector3> IndexedModel::getNormals() const
{
	return normals;
}

std::vector<unsigned int> IndexedModel::getIndices()
{
	return indices;
}