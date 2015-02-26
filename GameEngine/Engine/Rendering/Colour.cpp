#include <math.h>
#include <stdlib.h>

#include "../Core/Math3D.h"
#include "Colour.h"

Colour::Colour()
	: Vector4(0, 0, 0, 255)
{
	calculateCMYK();
}

Colour::Colour(float r, float g, float b, float a)
	: Vector4(r, g, b, a)
{
	calculateCMYK();
}

Colour Colour::hexToRGBA(int hex) const
{
	int r = (hex >> 24) & 0xFF;
	int g = (hex >> 16) & 0xFF;
	int b = (hex >> 8) & 0xFF;
	int a = (hex >> 0) & 0xFF;

	return Colour((float)r, (float)g, (float)b, (float)a);
}

Colour::Colour(int hex)
{
	Colour c = hexToRGBA(hex);
	x = c.getR();
	y = c.getG();
	z = c.getB();
	w = c.getA();

	calculateCMYK();
}

void Colour::calculateCMYK()
{
	float r1 = x / 255.0f;
	float g1 = y / 255.0f;
	float b1 = z / 255.0f;

	black = 1 - fmaxf(r1, fmaxf(g1, b1));
	cyan = (1 - r1 - black) / (1 - black);
	magenta = (1 - g1 - black) / (1 - black);
	yellow = (1 - b1 - black) / (1 - black);

	cyan *= 100.0f;
	magenta *= 100.0f;
	yellow *= 100.0f;
	black *= 100.0f;
}

int Colour::RGBAtoHex(float r, float g, float b, float a)
{
	return ((int)x << 24) + ((int)y << 16) + ((int)z << 8) + (int)w;
}

Colour Colour::toGLSLVec4() const
{
	return Colour(x / 255.0f, y / 255.0f, z / 255.0f, w / 255.0f);
}

Vector3 Colour::getRGB() const
{
	return Vector3(x, y, z) * w;
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

float Colour::getCyan() const
{
	return cyan;
}

float Colour::getMagenta() const
{
	return magenta;
}

float Colour::getYellow() const
{
	return yellow;
}

float Colour::getKey() const
{
	return black;
}

Vector4 Colour::getCMYK() const
{
	return Vector4(cyan, magenta, yellow, black);
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

Colour Colour::operator *(const Colour& c) const
{
	//return Colour(x * c.x, y * c.y, z * c.z, w * c.w);
	return Colour(x * c.x, y * c.y, z * c.z, w * c.w);
}

Colour getRandomColour()
{
	int r = random(0, 255);
	int g = random(0, 255);
	int b = random(0, 255);

	return Colour((float)r, (float)g, (float)b);
}

Colour getRandomColour(Colour mix)
{
	Colour c = getRandomColour();

	c.setR((c.getR() + mix.getR()) / 2);
	c.setG((c.getG() + mix.getG()) / 2);
	c.setB((c.getB() + mix.getB()) / 2);

	return c;
}