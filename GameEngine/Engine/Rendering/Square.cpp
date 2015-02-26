#include "Shape.h"

Square::Square(const Colour& colour, const Material& material) :
	Shape(colour, material)
{
	m_indexedModel.addVertex( 1,  1, 0);
	m_indexedModel.addVertex( 1, -1, 0);
	m_indexedModel.addVertex(-1,  1, 0);
	m_indexedModel.addVertex(-1, -1, 0);
	
	m_indexedModel.addFace(1, 0, 3);
	m_indexedModel.addFace(2, 3, 0);
	
	finalize("square");
}