#include "Shape.h"

Square::Square(const Colour& colour, const Material& material) :
	Shape(colour, material)
{
	m_indexedModel.addVertex(1.0f, 1.0f, 0.0f);
	m_indexedModel.addVertex(1.0f, -1.0f, 0.0f);
	m_indexedModel.addVertex(-1.0f, 1.0f, 0.0f);
	m_indexedModel.addVertex(-1.0f, -1.0f, 0.0f);
	
	m_indexedModel.addFace(1, 0, 3);
	m_indexedModel.addFace(2, 3, 0);
	
	finalize("square");
}