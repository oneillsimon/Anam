#include <algorithm>
#include <cmath>

#include "Math3D.h"

Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4::~Vector4()
{
}

void Vector4::registerMembers(const std::string& namespace_, lua_State* luaState)
{
	luabridge::getGlobalNamespace(luaState)
		.beginNamespace(namespace_.c_str())
		.beginClass<Vector4>("Vector4")
		.addConstructor<void(*)(const float&, const float&, const float&, const float&)>()
		.addFunction("length", &Vector4::length)
		.addFunction("normalised", &Vector4::normalised)
		.addFunction("getX", &Vector4::getX)
		.addFunction("getY", &Vector4::getY)
		.addFunction("getZ", &Vector4::getZ)
		.addFunction("getW", &Vector4::getW)
		.addFunction("setX", &Vector4::setX)
		.addFunction("setY", &Vector4::setY)
		.addFunction("setZ", &Vector4::setZ)
		.addFunction("setW", &Vector4::setW)
		.endClass()
		.endNamespace();
}

float Vector4::length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

float Vector4::max() const
{
	return std::max(x, std::max(y, std::max(z, w)));
}

Vector4 Vector4::normalised() const
{
	float l = length();
	return Vector4(x / l, y / l, z / l, w / l);
}

float Vector4::getX() const
{
	return x;
}

float Vector4::getY() const
{
	return y;
}

float Vector4::getZ() const
{
	return z;
}

float Vector4::getW() const
{
	return w;
}

void Vector4::setX(float x)
{
	this->x = x;
}

void Vector4::setY(float y)
{
	this->y = y;
}

void Vector4::setZ(float z)
{
	this->z = z;
}

void Vector4::setW(float w)
{
	this->w = w;
}