#include <vector>
#include <cassert>
#include <iostream>
#include <GL\glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Core/Profiling.h"
#include "Mesh.h"

std::map<std::string, MeshData*>Mesh::resourceMap;

IndexedModel::IndexedModel(const std::vector<unsigned int> indices, std::vector<Vector3> positions, std::vector<Vector2> texCoords, std::vector<Vector3> normals, std::vector<Vector3> tangents)
{
	m_indices = indices;
	m_positions = positions;
	m_texCoords = texCoords;
	m_normals = normals;
	m_tangents = tangents;
}

bool IndexedModel::isValid() const
{
	return m_positions.size() == m_texCoords.size() &&
		   m_texCoords.size() == m_normals.size() &&
		   m_normals.size() == m_tangents.size();
}

void IndexedModel::addVertex(const Vector3& v)
{
	m_positions.push_back(v);
}

void IndexedModel::addVertex(int x, int y, int z)
{
	addVertex(Vector3((float)x, (float)y, (float)z));
}

void IndexedModel::addTexCoord(const Vector2& v)
{
	m_texCoords.push_back(v);
}

void IndexedModel::addTexCoord(int x, int y)
{
	addTexCoord(Vector2((float)x, (float)y));
}

void IndexedModel::addNormal(const Vector3& v)
{
	m_normals.push_back(v);
}

void IndexedModel::addNormal(int x, int y, int z)
{
	addNormal(Vector3((float)x, (float)y, (float)z));
}

void IndexedModel::addTangent(const Vector3& v)
{
	m_tangents.push_back(v);
}

void IndexedModel::addTangent(int x, int y, int z)
{
	addTangent(Vector3((float)x, (float)y, (float)z));
}

IndexedModel IndexedModel::finalize()
{
	if(isValid())
	{
		return *this;
	}

	if(m_texCoords.size() == 0)
	{
		for(unsigned int i = 0; i < m_positions.size(); i++)
		{
			m_texCoords.push_back(Vector2());
		}
	}

	if(m_normals.size() == 0)
	{
		calculateNormals();
	}

	if(m_tangents.size() == 0)
	{
		calculateTangents();
	}

	return *this;
}

void IndexedModel::addFace(unsigned int vertexIndex0, unsigned int vertexIndex1, unsigned int vertexIndex2)
{
	m_indices.push_back(vertexIndex0);
	m_indices.push_back(vertexIndex1);
	m_indices.push_back(vertexIndex2);
}

void IndexedModel::calculateNormals()
{
	m_normals.clear();
	m_normals.reserve(m_positions.size());

	for(unsigned int i = 0; i < m_positions.size(); i++)
	{
		m_normals.push_back(Vector3());
	}

	for(unsigned int i = 0; i < m_indices.size(); i += 3)
	{
		int i0 = m_indices[i];
		int i1 = m_indices[i + 1];
		int i2 = m_indices[i + 2];

		Vector3 v1 = m_positions[i1] - m_positions[i0];
		Vector3 v2 = m_positions[i2] - m_positions[i0];

		Vector3 normal = v1.cross(v2).normalised();

		m_normals[i0] = m_normals[i0] + normal;
		m_normals[i1] = m_normals[i1] + normal;
		m_normals[i2] = m_normals[i2] + normal;
	}

	for(unsigned int i = 0; i < m_normals.size(); i++)
	{
		m_normals[i] = m_normals[i].normalised();
	}
}

void IndexedModel::calculateTangents()
{
	m_tangents.clear();
	m_tangents.reserve(m_positions.size());

	for(unsigned int i = 0; i < m_positions.size(); i++)
	{
		m_tangents.push_back(Vector3());
	}

	for(unsigned int i = 0; i < m_indices.size(); i += 3)
	{
		int i0 = m_indices[i];
		int i1 = m_indices[i + 1];
		int i2 = m_indices[i + 2];

		Vector3 edge1 = m_positions[i1] - m_positions[i0];
		Vector3 edge2 = m_positions[i2] - m_positions[i0];

		float deltaU1 = m_texCoords[i1].getX() - m_texCoords[i0].getX();
		float deltaU2 = m_texCoords[i2].getX() - m_texCoords[i0].getX();
		float deltaV1 = m_texCoords[i1].getY() - m_texCoords[i0].getY();
		float deltaV2 = m_texCoords[i2].getY() - m_texCoords[i0].getY();

		float dividend = (deltaU1 * deltaV2 - deltaU2 * deltaV1);

		float f = 0;

		if(dividend == 0.0f)
		{
			f = 0.0f;
		}
		else
		{
			f = 1.0f / dividend;
		}

		Vector3 tangent = Vector3();

		tangent.setX(f * (deltaV2 * edge1.getX() - deltaV1 * edge2.getX()));
		tangent.setY(f * (deltaV2 * edge1.getY() - deltaV1 * edge2.getY()));
		tangent.setZ(f * (deltaV2 * edge1.getZ() - deltaV1 * edge2.getZ()));

		m_tangents[i0] += tangent;
		m_tangents[i1] += tangent;
		m_tangents[i2] += tangent;
	}

	for(unsigned int i = 0; i < m_tangents.size(); i++)
	{
		m_tangents[i] = m_tangents[i].normalised();
	}
}

void IndexedModel::addToTexCoords(int index, const Vector2& value)
{
	m_texCoords[index] = m_texCoords[index] + value;
}

const std::vector<unsigned int>& IndexedModel::getIndices() const
{
	return m_indices;
}

const std::vector<Vector3>& IndexedModel::getPositions() const
{
	return m_positions;
}

const std::vector<Vector2>& IndexedModel::getTexCoords() const
{
	return m_texCoords;
}

const std::vector<Vector3>& IndexedModel::getNormals() const
{
	return m_normals;
}

const std::vector<Vector3>& IndexedModel::getTangents() const
{
	return m_tangents;
}

MeshData::MeshData(const IndexedModel& model) :
	ReferenceCounter(),
	m_drawCount(model.getIndices().size()),
	m_wireFrameShader("forward-colour")
{
	if(!model.isValid())
	{
		fprintf(stderr, "Invalid meshdata");
	}

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.getPositions().size() * sizeof(model.getPositions()[0]), &model.getPositions()[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXTCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.getTexCoords().size() * sizeof(model.getTexCoords()[0]), &model.getTexCoords()[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.getNormals().size() * sizeof(model.getNormals()[0]), &model.getNormals()[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TANGENT_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.getTangents().size() * sizeof(model.getTangents()[0]), &model.getTangents()[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.getIndices().size() * sizeof(model.getIndices()[0]), &model.getIndices()[0], GL_STATIC_DRAW);
}

MeshData::~MeshData()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void MeshData::updateTextureBuffer(const IndexedModel& model)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXTCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.getTexCoords().size() * sizeof(model.getTexCoords()[0]), &model.getTexCoords()[0], GL_STATIC_DRAW);
}

void MeshData::draw(int primitive) const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(m_vertexArrayObject);

#if PROFILING_DISABLE_MESH_DRAWING == 0
	glDrawElements(primitive, m_drawCount, GL_UNSIGNED_INT, 0);
#endif
}

void MeshData::drawWireFrame() const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonOffset(-10, -10);
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
}

Shader MeshData::getWireFrameShader() const
{
	return m_wireFrameShader;
}

Mesh::Mesh()
{
	m_fileName = "";
	m_meshData = 0;
}

Mesh::Mesh(const std::string& filename)
{
	m_fileName = filename;
	m_meshData = 0;

	std::map<std::string, MeshData*>::const_iterator it = resourceMap.find(filename);

	if(it != resourceMap.end())
	{
		m_meshData = it->second;
		m_meshData->addReference();
	}
	else
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(("./res/models/" + filename).c_str(),
												 aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if(!scene)
		{
			fprintf(stderr, "Mesh load failed: %s\n", filename);
		}

		const aiMesh* model = scene->mMeshes[0];

		std::vector<unsigned int> indices;
		std::vector<Vector3> positions;
		std::vector<Vector2> texCoords;
		std::vector<Vector3> normals;
		std::vector<Vector3> tangents;

		const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);

		for(unsigned int i = 0; i < model->mNumVertices; i++)
		{
			const aiVector3D pos = model->mVertices[i];
			const aiVector3D normal = model->mNormals[i];
			const aiVector3D texCoord = model->HasTextureCoords(0) ? model->mTextureCoords[0][i] : aiZeroVector;
			const aiVector3D tangent = model->mTangents[i];

			positions.push_back(Vector3(pos.x, pos.y, pos.z));
			texCoords.push_back(Vector2(texCoord.x, texCoord.y));
			normals.push_back(Vector3(normal.x, normal.y, normal.z));
			tangents.push_back(Vector3(tangent.x, tangent.y, tangent.z));
		}

		for(unsigned int i = 0; i < model->mNumFaces; i++)
		{
			const aiFace& face = model->mFaces[i];
			assert(face.mNumIndices == 3);

			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		m_meshData = new MeshData(IndexedModel(indices, positions, texCoords, normals, tangents));
		resourceMap.insert(std::pair<std::string, MeshData*>(filename, m_meshData));
	}
}

Mesh::Mesh(const std::string& meshName, const IndexedModel& model)
{
	m_fileName = meshName;

	std::map<std::string, MeshData*>::const_iterator it = resourceMap.find(meshName);

	if(it != resourceMap.end())
	{
		fprintf(stderr, "Error adding Mesh, it already exists");
	}
	else
	{
		m_meshData = new MeshData(model);
		resourceMap.insert(std::pair<std::string, MeshData*>(meshName, m_meshData));
	}
}

Mesh::Mesh(const Mesh& mesh)
{
	m_fileName = mesh.m_fileName;
	m_meshData = mesh.m_meshData;
	m_meshData->addReference();
}

Mesh::~Mesh()
{
	if(m_meshData && m_meshData->removeReference())
	{
		if(m_fileName.length() > 0)
		{
			resourceMap.erase(m_fileName);
		}

		delete m_meshData;
	}
}

void Mesh::draw(int primitive) const
{
	m_meshData->draw(primitive);
}

void Mesh::drawWireFrame() const
{
	m_meshData->drawWireFrame();
}

Shader Mesh::getWireFrameShader() const
{
	return m_meshData->getWireFrameShader();
}