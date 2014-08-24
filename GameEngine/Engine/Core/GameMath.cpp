#define _USE_MATH_DEFINES

#include <cmath>
#include <stdlib.h>

#include "GameMath.h"

double GameMath::toRadians(double d)
{
	return d * M_PI / 180;
}

float GameMath::random(float min, float max)
{
	return min + (rand() % (int)(max - min + 1));
}