#include "Shape.h"

Triangle::Triangle(const Colour& colour, const Material& material) :
	Shape(colour, material)
{
	m_indexedModel.addVertex( 0,  1, 0);
	m_indexedModel.addVertex( 1, -1, 0);
	m_indexedModel.addVertex(-1, -1, 0);
	
	m_indexedModel.addFace(0, 1, 2);
	m_indexedModel.addFace(2, 1, 0);
	
	finalize("triangle");
}