#ifndef SHAPE_H
#define SHAPE_H

#include "Mesh.h"

class Shape : public Mesh
{
protected:
	Material m_material;
	IndexedModel m_indexedModel;

public:
	Shape(const Colour& colour, const Material& material = Material("shapeMaterial", TEXTURE_BLANK));

	Material getMaterial() const;
};

class Square : public Shape
{
public:
	Square(const Colour& colour, const Material& material = Material("shapeMaterial", TEXTURE_BLANK));
};

#endif