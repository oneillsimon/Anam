#include <vector>
#include <cassert>
#include <iostream>
#include <GL\glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "OBJLoader.h"
#include "Mesh.h"

std::map<std::string, MeshData*>Mesh::resourceMap;

MeshData::MeshData(int indexSize) : ReferenceCounter()
{
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	size = indexSize;
}

MeshData::~MeshData()
{
	if(vbo)
	{
		glDeleteBuffers(1, &vbo);
	}

	if(ibo)
	{
		glDeleteBuffers(1, &ibo);
	}
}

Mesh::Mesh()
{
}

Mesh::Mesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals)
{
	m_fileName = "";
	initMesh(vertices, vertSize, indices, indexSize, calcNormals);
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

		const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if(!scene)
		{
			fprintf(stderr, "Mesh load failed: %s\n", filename);
		}

		const aiMesh* model = scene->mMeshes[0];

		std::vector<Vertex> vertices;
		std::vector<int> indices;

		const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);

		for(unsigned int i = 0; i < model->mNumVertices; i++)
		{
			const aiVector3D* pos = &(model->mVertices[i]);
			const aiVector3D* nor = &(model->mNormals[i]);
			const aiVector3D* tCor = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][i]) : &aiZeroVector;
			const aiVector3D* tang = &(model->mTangents[i]);

			Vertex vertex(Vector3(pos->x, pos->y, pos->z), Vector2(tCor->x, tCor->y), Vector3(nor->x, nor->y, nor->z), Vector3(tang->x, tang->y, tang->z));
			vertices.push_back(vertex);
		}

		for(unsigned int i = 0; i < model->mNumFaces; i++)
		{
			const aiFace& face = model->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		initMesh(&vertices[0], vertices.size(), (int*)&indices[0], indices.size(), false);
		resourceMap.insert(std::pair<std::string, MeshData*>(m_fileName, m_meshData));
	}
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

void Mesh::initMesh(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals)
{
	m_meshData = new MeshData(indexSize);

	if(calcNormals)
	{
		calculateNormals(vertices, vertSize, indices, indexSize);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->getVbo());
	glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshData->getIbo());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(int), indices, GL_STATIC_DRAW);
}

void Mesh::draw()
{
	draw(GL_TRIANGLES);
}

void Mesh::draw(int gl_primitives)
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, m_meshData->getVbo());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3) + sizeof(Vector2)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3) + sizeof(Vector2) + sizeof(Vector3)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshData->getIbo());
	glDrawElements(gl_primitives, m_meshData->getSize(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void Mesh::calculateNormals(Vertex* vertices, int vertSize, int* indices, int indexSize)
{
	for(int i = 0; i < indexSize; i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		Vector3 v1 = vertices[i1].getPosition() - vertices[i0].getPosition();
		Vector3 v2 = vertices[i2].getPosition() - vertices[i0].getPosition();

		Vector3 normal = v1.cross(v2).normalised();

		vertices[i0].setNormal(vertices[i0].getNormal() + normal);
		vertices[i1].setNormal(vertices[i1].getNormal() + normal);
		vertices[i2].setNormal(vertices[i2].getNormal() + normal);
	}

	for(int i = 0; i < vertSize; i++)
	{
		vertices[i].setNormal(vertices[i].getNormal().normalised());
	}
}

int MeshData::getVbo()
{
	return vbo;
}

int MeshData::getIbo()
{
	return ibo;
}

int MeshData::getSize()
{
	return size;
}