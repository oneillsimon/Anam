#include <algorithm>
#include <cmath>

#include "Math3D.h"

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::~Vector2()
{
}

void Vector2::registerMembers(const std::string& namespace_, lua_State* luaState)
{
	luabridge::getGlobalNamespace(luaState)
		.beginNamespace(namespace_.c_str())
		.beginClass<Vector2>("Vector2")
		.addConstructor<void(*)(const float&, const float&)>()
		.addFunction("length", &Vector2::length)
		.addFunction("dot", &Vector2::dot)
		.addFunction("cross", &Vector2::cross)
		.addFunction("normalised", &Vector2::normalised)
		.addFunction("inversed", &Vector2::inversed)
		.addFunction("rotate", &Vector2::rotate)
		.addFunction("lerp", &Vector2::lerp)
		.addFunction("getX", &Vector2::getX)
		.addFunction("getY", &Vector2::getY)
		.addFunction("setX", &Vector2::setX)
		.addFunction("setY", &Vector2::setY)
		.endClass()
		.endNamespace();
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
	double radians = toRadians(angle);
	double cosine = cos(radians);
	double sine = sin(radians);

	return Vector2(x * (float)cosine - y * (float)sine, x * (float)sine + y * (float)cosine);
}

Vector2 Vector2::lerp(const Vector2& destination, float lerpFactor) const
{
	return destination - *this * lerpFactor + *this;
}

//void Vector2::registerMembers(const std::string& namespace_, lua_State* luaState)
//{
//	luabridge::getGlobalNamespace(luaState)
//		.beginNamespace(namespace_.c_str())
//		.beginClass<Vector2>("Vector2")
//		.addConstructor<void(*)(const float&, const float&)>()
//		.addFunction("length", &Vector2::length)
//		.addFunction("dot", &Vector2::dot)
//		.addFunction("cross", &Vector2::cross)
//		.addFunction("normalised", &Vector2::normalised)
//		.addFunction("inversed", &Vector2::inversed)
//		.addFunction("rotate", &Vector2::rotate)
//		.addFunction("lerp", &Vector2::lerp)
//		.addFunction("getX", &Vector2::getX)
//		.addFunction("getY", &Vector2::getY)
//		.addFunction("setX", &Vector2::setX)
//		.addFunction("setY", &Vector2::setY)
//		.endClass()
//		.endNamespace();
//}

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

bool Vector2::operator ==(const Vector2& v) const
{
	return x == v.getX() && y == v.getY();
}

bool Vector2::operator !=(const Vector2& v) const
{
	return x != v.getX() || y != v.getY();
}