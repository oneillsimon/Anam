#include <cmath>
#include <algorithm>
#include "3DMath.h"

const Vector3 Vector3::ZERO = Vector3(0, 0, 0);
const Vector3 Vector3::ONE = Vector3(1, 1, 1);

Vector3::Vector3()
{
	Vector3(0, 0, 0);
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::~Vector3()
{
}

float Vector3::length() const
{
	return sqrtf(x * x + y * y + z * z);
}

float Vector3::dot(const Vector3& v) const
{
	return x * v.getX() + y * v.getY() + z * v.getZ();
}

float Vector3::max() const
{
	return std::max(x, std::max(y, z));
}

Vector3 Vector3::cross(const Vector3& v) const
{
	float x_ = y * v.getZ() - z * v.getY();
	float y_ = z * v.getX() - x * v.getZ();
	float z_ = x * v.getY() - y * v.getX();

	return Vector3(x_, y_, z_);
}

Vector3 Vector3::normalised() const
{
	const float l = length();
	return Vector3(x / l, y / l, z / l);
}

Vector3 Vector3::rotate(const Vector3& axis, float angle) const
{
	float sinAngle = (float)sin(-angle);
	float cosAngle = (float)cos(-angle);

	return this->cross(axis * sinAngle) +
		   ((*this * cosAngle) +
		   (axis * this->dot(axis * (1 - cosAngle))));

}

Vector3 Vector3::rotate(const Quaternion& rotation) const
{
	Quaternion conjugate = rotation.conjugate();
	Quaternion w = rotation * (*this) * conjugate;

	return Vector3(w.getX(), w.getY(), w.getZ());
}

Vector3 Vector3::lerp(const Vector3& destination, float lerpFactor) const
{
	return destination - *this * lerpFactor + *this;
}

float Vector3::getX() const
{
	return x;
}

float Vector3::getY() const
{
	return y;
}

float Vector3::getZ() const
{
	return z;
}

void Vector3::setX(float x)
{
	this->x = x;
}

void Vector3::setY(float y)
{
	this->y = y;
}

void Vector3::setZ(float z)
{
	this->z = z;
}

Vector3 Vector3::operator +(const Vector3& v) const
{
	return Vector3(x + v.getX(), y + v.getY(), z + v.getZ());
}

Vector3 Vector3::operator +(float f) const
{
	return Vector3(x + f, y + f, z + f);
}

Vector3 Vector3::operator -(const Vector3& v) const
{
	return Vector3(x - v.getX(), y - v.getY(), z - v.getZ());
}

Vector3 Vector3::operator -(float f) const
{
	return Vector3(x - f, y - f, z - f);
}

Vector3 Vector3::operator *(const Vector3& v) const
{
	return Vector3(x * v.getX(), y * v.getY(), z * v.getZ());
}

Vector3 Vector3::operator *(float f) const
{
	return Vector3(x * f, y * f, z * f);
}

Vector3 Vector3::operator /(const Vector3& v) const
{
	return Vector3(x / v.getX(), y / v.getY(), z / v.getZ());
}

Vector3 Vector3::operator /(float f) const
{
	return Vector3(x / f, y / f, z / f);
}

bool Vector3::operator !=(const Vector3& v) const
{
	return x != v.getX() || y != v.getY() || z != v.getZ();
}