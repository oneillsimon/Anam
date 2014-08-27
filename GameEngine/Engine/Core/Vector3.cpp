#include <algorithm>
#include <cmath>

#include "Math3D.h"

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
	return sqrtf(squareLength());
}

float Vector3::squareLength() const
{
	return x * x + y * y + z * z;
}

float Vector3::dot(const Vector3& v) const
{
	return x * v.getX() + y * v.getY() + z * v.getZ();
}

float Vector3::max() const
{
	return std::max(x, std::max(y, z));
}

Vector3 Vector3::max(const Vector3& v) const
{
	Vector3 result;

	result.x = std::max(x, v.x);
	result.y = std::max(y, v.y);
	result.z = std::max(z, v.z);

	return result;
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
	float l = length();
	return Vector3(x / l, y / l, z / l);
}

Vector3 Vector3::inversed() const
{
	return Vector3(-x, -y, -z);
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

Vector3 Vector3::reflect(const Vector3& normal) const
{
	return *this - (normal * (this->dot(normal) * 2));
}

Vector3 Vector3::absolute()
{
	return Vector3(abs(x), abs(y), abs(z));
}

void Vector3::addScaledVector3(const Vector3& v, float scale)
{
	x += (v.getX() * scale);
	y += (v.getY() * scale);
	z += (v.getZ() * scale);
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

Vector3 Vector3::operator =(const Vector3& v) const
{
	x = v.getX();
	y = v.getY();
	z = v.getZ();

	return *this;
}

Vector3 Vector3::operator +(const Vector3& v) const
{
	return Vector3(x + v.getX(), y + v.getY(), z + v.getZ());
}

Vector3 Vector3::operator -(const Vector3& v) const
{
	return Vector3(x - v.getX(), y - v.getY(), z - v.getZ());
}

Vector3 Vector3::operator *(const Vector3& v) const
{
	return Vector3(x * v.getX(), y * v.getY(), z * v.getZ());
}

Vector3 Vector3::operator /(const Vector3& v) const
{
	return Vector3(x / v.getX(), y / v.getY(), z / v.getZ());
}

void Vector3::operator +=(const Vector3& v) const
{
	x += v.getX();
	y += v.getY();
	z += v.getZ();
}

void Vector3::operator -=(const Vector3& v) const
{
	x -= v.getX();
	y -= v.getY();
	z -= v.getZ();
}

void Vector3::operator *=(const Vector3& v) const
{
	x *= v.getX();
	y *= v.getY();
	z *= v.getZ();
}

void Vector3::operator /=(const Vector3& v) const
{
	x /= v.getX();
	y /= v.getY();
	z /= v.getZ();
}

Vector3 Vector3::operator +(float f) const
{
	return Vector3(x + f, y + f, z + f);
}

Vector3 Vector3::operator -(float f) const
{
	return Vector3(x - f, y - f, z - f);
}

Vector3 Vector3::operator *(float f) const
{
	return Vector3(x * f, y * f, z * f);
}

Vector3 Vector3::operator /(float f) const
{
	return Vector3(x / f, y / f, z / f);
}

void Vector3::operator +=(float f) const
{
	x += f;
	y += f;
	z += f;
}

void Vector3::operator -=(float f) const
{
	x -= f;
	y -= f;
	z -= f;
}

void Vector3::operator *=(float f) const
{
	x *= f;
	y *= f;
	z *= f;
}

void Vector3::operator /=(float f) const
{
	x /= f;
	y /= f;
	z /= f;
}

bool Vector3::operator ==(const Vector3& v) const
{
	return x == v.getX() && y == v.getY() && z == v.getZ();
}

bool Vector3::operator !=(const Vector3& v) const
{
	return x != v.getX() || y != v.getY() || z != v.getZ();
}