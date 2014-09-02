#ifndef SHAPE_H
#define SHAPE_H

#include "Mesh.h"

class Shape : public Mesh
{
protected:
	Material m_material;
	IndexedModel m_indexedModel;

	void finalize(const std::string& shapeName);

public:
	Shape(const Colour& colour, const Material& material = Material("shapeMaterial", TEXTURE_BLANK));

	Material getMaterial() const;
};

class Square : public Shape
{
public:
	Square(const Colour& colour, const Material& material = MATERIAL_DEFAULT);
};

class Triangle : public Shape
{
public:
	Triangle(const Colour& colour, const Material& material = MATERIAL_DEFAULT);
};

#endif