#include "Shape.h"

Shape::Shape(const Colour& colour, const Material& material) :
	Mesh(),
	m_material(material)
{
	m_material.setVector4("colour", colour);
}

Material Shape::getMaterial() const
{
	return m_material;
}