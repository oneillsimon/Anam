#include "Shape.h"

Square::Square(const Colour& colour, const Material& material) :
	Shape(colour, material)
{
	Vector2 _3_00(0.0f, 0.0f);
	Vector2 _1_10(1.0f, 0.0f);
	Vector2 _2_01(0.0f, 1.0f);
	Vector2 _0_11(1.0f, 1.0f);

	m_indexedModel.addVertex(1.0f, 1.0f, 0.0f);
	m_indexedModel.addVertex(1.0f, -1.0f, 0.0f);
	m_indexedModel.addVertex(-1.0f, 1.0f, 0.0f);
	m_indexedModel.addVertex(-1.0f, -1.0f, 0.0f);
	
	m_indexedModel.addTexCoord(_0_11);
	m_indexedModel.addTexCoord(_1_10);
	m_indexedModel.addTexCoord(_2_01);
	m_indexedModel.addTexCoord(_3_00);
	
	m_indexedModel.addFace(1, 0, 3);
	m_indexedModel.addFace(2, 3, 0);
	
	m_indexedModel = m_indexedModel.finalize();

	m_meshData = new MeshData(m_indexedModel);
	resourceMap.insert(std::pair<std::string, MeshData*>("square", m_meshData));
}