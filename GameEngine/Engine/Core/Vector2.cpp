#include <cmath>
#include <algorithm>
#include "GameMath.h"
#include "Math3D.h"

const Vector2 Vector2::ZERO = Vector2(0, 0);
const Vector2 Vector2::ONE = Vector2(1, 1);

Vector2::Vector2()
{
	Vector2(0, 0);
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::~Vector2()
{
}

float Vector2::length() const
{
	return sqrtf(x * x + y * y);
}

float Vector2::dot(const Vector2& v) const
{
	return x * v.getX() + y * v.getY();
}

float Vector2::max() const
{
	return std::max(x, y);
}

float Vector2::cross(const Vector2& v) const
{
	return x * v.getY() - y * v.getX();
}

Vector2 Vector2::normalised() const
{
	float l = length();

	return Vector2(x / l, y / l);
}

Vector2 Vector2::inversed() const
{
	return Vector2(-x, -y);
}

Vector2 Vector2::rotate(float angle)
{
	double radians = GameMath::toRadians(angle);
	double cosine = cos(radians);
	double sine = sin(radians);

	return Vector2(x * (float)cosine - y * (float)sine, x * (float)sine + y * (float)cosine);
}

Vector2 Vector2::lerp(const Vector2& destination, float lerpFactor) const
{
	return destination - *this * lerpFactor + *this;
}

float Vector2::getX() const
{
	return x;
}

float Vector2::getY() const
{
	return y;
}

void Vector2::setX(float x)
{
	this->x = x;
}

void Vector2::setY(float y)
{
	this->y = y;
}

Vector2 Vector2::operator +(const Vector2& v) const
{
	return Vector2(x + v.getX(), y + v.getY());
}

Vector2 Vector2::operator +(float f) const
{
	return Vector2(x + f, y + f);
}

Vector2 Vector2::operator -(const Vector2& v) const
{
	return Vector2(x - v.getX(), y - v.getY());
}

Vector2 Vector2::operator -(float f) const
{
	return Vector2(x - f, y - f);
}

Vector2 Vector2::operator *(const Vector2& v) const
{
	return Vector2(x * v.getX(), y * v.getY());
}

Vector2 Vector2::operator *(float f) const
{
	return Vector2(x * f, y * f);
}

Vector2 Vector2::operator /(const Vector2& v) const
{
	return Vector2(x / v.getX(), y / v.getY());
}

Vector2 Vector2::operator /(float f) const
{
	return Vector2(x / f, y / f);
}