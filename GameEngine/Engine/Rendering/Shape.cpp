#include "Shape.h"

Shape::Shape(const Colour& colour, const Material& material) :
	Mesh(),
	m_material(material)
{
	m_material.setVector4("colour", colour);
}

void Shape::finalize(const std::string& shapeName)
{
	m_indexedModel = m_indexedModel.finalize();

	m_meshData = new MeshData(m_indexedModel);
	resourceMap.insert(std::pair<std::string, MeshData*>(shapeName, m_meshData));
}

Material Shape::getMaterial() const
{
	return m_material;
}