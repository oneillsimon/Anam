#include <cmath>

#include "Math3D.h"

Quaternion::Quaternion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion::Quaternion(const Vector3& axis, float angle)
{
	float sinHalfAngle = sinf(angle / 2);
	float cosHalfAngle = cosf(angle / 2);

	x = axis.getX() * sinHalfAngle;
	y = axis.getY() * sinHalfAngle;
	z = axis.getZ() * sinHalfAngle;
	w = cosHalfAngle;
}

Quaternion::Quaternion(Matrix4 rotation)
{
	float trace = rotation.getAt(0, 0) + rotation.getAt(1, 1) + rotation.getAt(2, 2);

	if(trace > 0)
	{
		float s = 0.5f / sqrtf(trace + 1.0f);
		w = 0.25f / s;
		x = (rotation.getAt(1, 2) - rotation.getAt(2, 1)) * s;
		y = (rotation.getAt(1, 2) - rotation.getAt(2, 1)) * s;
		x = (rotation.getAt(1, 2) - rotation.getAt(2, 1)) * s;
	}
	else
	{
		if(rotation.getAt(0, 0) > rotation.getAt(1, 1) && rotation.getAt(0, 0) > rotation.getAt(2, 2))
		{
			float s = 2.0f * (float)sqrtf(1.0f + rotation.getAt(0, 0) - rotation.getAt(1, 1) - rotation.getAt(2, 2));
			w = (rotation.getAt(1, 2) - rotation.getAt(2, 1)) / s;
			x = 0.25f * s;
			y = (rotation.getAt(1, 0) - rotation.getAt(0, 1)) / s;
			z = (rotation.getAt(2, 0) - rotation.getAt(0, 2)) / s;
		}
		else if(rotation.getAt(1, 1) > rotation.getAt(2, 2))
		{
			float s = 2.0f * (float)sqrtf(1.0f + rotation.getAt(1, 1) - rotation.getAt(0, 0) - rotation.getAt(2, 2));
			w = (rotation.getAt(2, 0) - rotation.getAt(0, 2)) / s;
			x = (rotation.getAt(1, 0) + rotation.getAt(0, 1)) / s;
			y = 0.25f * s;
			z = (rotation.getAt(2, 1) + rotation.getAt(1, 2)) / s;
		}
		else
		{
			float s = 2.0f * (float)sqrtf(1.0f + rotation.getAt(2, 2) - rotation.getAt(0, 0) - rotation.getAt(1, 1));
			w = (rotation.getAt(0, 1) - rotation.getAt(1, 0)) / s;
			x = (rotation.getAt(2, 0) - rotation.getAt(0, 2)) / s;
			y = (rotation.getAt(1, 2) - rotation.getAt(2, 1)) / s;
			z = 0.25f * s;
		}
	}
}

Quaternion::~Quaternion()
{
}

float Quaternion::length()
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

Quaternion Quaternion::normalised()
{
	float l = length();

	x /= l;
	y /= l;
	z /= l;
	w /= l;

	return *this;
}

Quaternion Quaternion::conjugate() const
{
	return Quaternion(-x, -y, -z, w);
}

float Quaternion::dot(const Quaternion& q) const
{
	return x * q.getX() + y * q.getY() + z * q.getZ() + w * q.getW();
}

Quaternion Quaternion::nlerp(Quaternion destination, float lerpFactor, bool shortest)
{
	Quaternion correctedDestination = destination;
		
	if(shortest && this->dot(destination) < 0)
	{
		correctedDestination = Quaternion(-destination.getX(), -destination.getY(), -destination.getZ(), -destination.getW());
	}

	return (correctedDestination - *this * lerpFactor + *this).normalised();
}

Quaternion Quaternion::slerp(Quaternion destination, float lerpFactor, bool shortest)
{
	const float epsilon = 1000.0f;
		
	float cos_ = this->dot(destination);
	Quaternion correctedDestination = destination;
	
	if(shortest && cos_ < 0)
	{
		cos_ = -cos_;
		correctedDestination = Quaternion(-destination.getX(), -destination.getY(), -destination.getZ(), -destination.getW());
	}
	
	if(abs(cos_) >= 1 - epsilon)
	{
		return nlerp(correctedDestination, lerpFactor, false);
	}
	
	float sin_ = sqrtf(1.0f - cos_ * cos_);
	float angle = atan2(sin_, cos_);
	float invSin = 1.0f / sin_;
	
	float srcFactor = sin((1.0f - lerpFactor) * angle) * invSin;
	float destFactor = sin(lerpFactor * angle) * invSin;

	return *this * srcFactor + correctedDestination * destFactor;
}

Matrix4 Quaternion::toRotationMatrix() const
{
	Vector3 f = Vector3(2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y));
	Vector3 u = Vector3(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z - w * x));
	Vector3 r = Vector3(1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y - w * z), 2.0f * (x * z + w * y));
	
	return Matrix4().initRotation(f, u, r);
}

Vector3 Quaternion::getForward() const
{
	return Vector3(0, 0, 1).rotate(*this);
}

Vector3 Quaternion::getBackward() const
{
	return Vector3(0, 0, -1).rotate(*this);
}

Vector3 Quaternion::getUp() const
{
	return Vector3(0, 1, 0).rotate(*this);
}

Vector3 Quaternion::getDown() const
{
	return Vector3(0, -1, 0).rotate(*this);
}

Vector3 Quaternion::getLeft() const
{
	return Vector3(-1, 0, 0).rotate(*this);
}

Vector3 Quaternion::getRight() const
{
	return Vector3(1, 0, 0).rotate(*this);
}

float Quaternion::getX() const
{
	return x;
}

float Quaternion::getY() const
{
	return y;
}

float Quaternion::getZ() const
{
	return z;
}

float Quaternion::getW() const
{
	return w;
}

Quaternion Quaternion::operator +(const Quaternion& q) const
{
	return Quaternion(x + q.getX(), y + q.getY(), z + q.getZ(), w + q.getW());
}

Quaternion Quaternion::operator -(const Quaternion& q) const
{
	return Quaternion(x - q.getX(), y - q.getY(), z - q.getZ(), w - q.getW());
}

Quaternion Quaternion::operator *(const Quaternion& q) const
{
	float x_ = (x * q.getW()) + (w * q.getX()) + (y * q.getZ()) - (z * q.getY());
	float y_ = (y * q.getW()) + (w * q.getY()) + (z * q.getX()) - (x * q.getZ());
	float z_ = (z * q.getW()) + (w * q.getZ()) + (x * q.getY()) - (y * q.getX());
	float w_ = (w * q.getW()) - (x * q.getX()) - (y * q.getY()) - (z * q.getZ());
		
	return Quaternion(x_, y_, z_, w_);
}

Quaternion Quaternion::operator *(const Vector3& v) const
{
	float w_ = (-x * v.getX()) - (y * v.getY()) - (z * v.getZ());
	float x_ = ( w * v.getX()) + (y * v.getZ()) - (z * v.getY());
	float y_ = ( w * v.getY()) + (z * v.getX()) - (x * v.getZ());
	float z_ = ( w * v.getZ()) + (x * v.getY()) - (y * v.getX());	

	return Quaternion(x_, y_, z_, w_);
}

Quaternion Quaternion::operator *(float f) const
{
	return Quaternion(x * f, y * f, z * f, w * f);
}

bool Quaternion::operator ==(const Quaternion& q) const
{
	return x == q.getX() && y == q.getY() && z == q.getZ() && w == q.getW();
}

bool Quaternion::operator !=(const Quaternion& q) const
{
	return x != q.getX() || y != q.getY() || z != q.getZ() || w != q.getW();
}