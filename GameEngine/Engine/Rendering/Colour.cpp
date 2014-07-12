#include "Colour.h"

Colour::Colour(int r, int g, int b)
{
	Colour(r, g, b, 1);
}

Colour::Colour(int r, int g, int b, int a)
{
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

int Colour::getRed() const
{
	return red;
}

int Colour::getGreen() const
{
	return green;
}

int Colour::getBlue() const
{
	return blue;
}

int Colour::getAlpha() const
{
	return alpha;
}

void Colour::setRed(int r)
{
	red = r;
}

void Colour::setGreen(int g)
{
	green = g;
}

void Colour::setBlue(int b)
{
	blue = b;
}

void Colour::setAlpha(int a)
{
	alpha = a;
}