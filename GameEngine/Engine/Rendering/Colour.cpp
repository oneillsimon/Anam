#include "Colour.h"

Colour::Colour()
	: Vector4(0, 0, 0, 0)
{
}

Colour::Colour(float r, float g, float b)
	: Vector4(r / 255.0f, g / 255.0f, b / 255.0f, 255.0f)
{
}

Colour::Colour(float r, float g, float b, float a)
	: Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f)
{
}

Vector3 Colour::getRGB() const
{
	return Vector3(x, y, z);
}

float Colour::getR() const
{
	return x;
}

float Colour::getG() const
{
	return y;
}

float Colour::getB() const
{
	return z;
}

float Colour::getA() const
{
	return w;
}

void Colour::setR(float r)
{
	this->x = r;
}

void Colour::setG(float g)
{
	this->y = g;
}

void Colour::setB(float b)
{
	this->z = b;
}

void Colour::setA(float a)
{
	this->w = a;
}