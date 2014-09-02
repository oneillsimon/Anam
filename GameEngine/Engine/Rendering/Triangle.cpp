#include "Shape.h"

Triangle::Triangle(const Colour& colour, const Material& material) :
	Shape(colour, material)
{
	m_indexedModel.addVertex(0.0f, 1.0f, 0.0f);
	m_indexedModel.addVertex(1.0f, -1.0f, 0.0f);
	m_indexedModel.addVertex(-1.0f, -1.0f, 0.0f);
	
	m_indexedModel.addFace(0, 1, 2);
	m_indexedModel.addFace(2, 1, 0);
	
	finalize("triangle");
}