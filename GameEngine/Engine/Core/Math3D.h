#ifndef MATH3D_H
#define MATH3D_H

#define PI 3.14159265358979323846264338327950288419716939937510
#define toRadians(x) (float)(x * PI / 180.0f)
#define random(min, max) min + (rand() % (int)(max - min + 1));

#define AXIS_X Vector3(1, 0, 0)
#define AXIS_Y Vector3(0, 1, 0)
#define AXIS_Z Vector3(0, 0, 1)
#define AXIS_XY Vector3(1, 1, 0)
#define AXIS_XZ Vector3(1, 0, 1)
#define AXIS_YZ Vector3(0, 1, 1)
#define AXIS_XYZ Vector3(1, 1, 1)

#include <string>

class Matrix3;
class Quaternion;

class Vector2
{
private:
	float x;
	float y;

public:
	static const Vector2 ZERO;
	static const Vector2 ONE;

	Vector2(float x = 0.0f, float y = 0.0f);
	~Vector2();

	float length() const;
	float dot(const Vector2& v) const;
	float max() const;
	float cross(const Vector2& v) const;
	Vector2 normalised() const;
	Vector2 inversed() const;
	Vector2 rotate(float angle);
	Vector2 lerp(const Vector2& destination, float lerpFactor) const;

	float getX() const;
	float getY() const;

	void setX(float x);
	void setY(float y);

	Vector2 operator +(const Vector2& v) const;
	Vector2 operator +(float f) const;
	Vector2 operator -(const Vector2& v) const;
	Vector2 operator -(float f) const;
	Vector2 operator *(const Vector2& v) const;
	Vector2 operator *(float f) const;
	Vector2 operator /(const Vector2& v) const;
	Vector2 operator /(float f) const;

	bool operator ==(const Vector2& v) const;
	bool operator !=(const Vector2& v) const;
};

class Vector3
{
private:
	mutable float x;
	mutable float y;
	mutable float z;

public:
	static void pv3(const Vector3& v, const std::string& msg = "")
	{
		printf("%s %.4f, %.4f, %.4f\n", msg.c_str(), v[0], v[1], v[2]);
	}

	static const Vector3 ZERO;
	static const Vector3 ONE;

	static const Vector3 GRAVITY;

	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	~Vector3();

	float length() const;
	float squareLength() const;
	float dot(const Vector3& v) const;
	float max() const;
	Vector3 max(const Vector3& v) const;
	Vector3 cross(const Vector3& v) const;
	Vector3 normalised() const;
	Vector3 inversed() const;
	Vector3 rotate(const Vector3& axis, float angle) const;
	Vector3 rotate(const Quaternion& rotation) const;
	Vector3 reflect(const Vector3& normal) const;
	Vector3 lerp(const Vector3& destination, float lerpFactor) const;
	Vector3 absolute();
	float scalarProduct(const Vector3& vector) const;
	Vector3 vectorProduct(const Vector3 &vector) const;
	void addScaledVector(const Vector3& vector, float scale);

	float getX() const;
	float getY() const;
	float getZ() const;

	void setX(float x);
	void setY(float y);
	void setZ(float z);

	Vector3 operator =(const Vector3& v) const;

	Vector3 operator +(const Vector3& v) const;
	Vector3 operator -(const Vector3& v) const;
	Vector3 operator *(const Vector3& v) const;
	Vector3 operator /(const Vector3& v) const;
	Vector3 operator %(const Vector3& v) const;

	void operator +=(const Vector3& v) const;
	void operator -=(const Vector3& v) const;
	void operator *=(const Vector3& v) const;
	void operator /=(const Vector3& v) const;

	Vector3 operator +(float f) const;
	Vector3 operator -(float f) const;
	Vector3 operator *(float f) const;
	Vector3 operator /(float) const;

	void operator +=(float f) const;
	void operator -=(float f) const;
	void operator *=(float f) const;
	void operator /=(float f) const;

	bool operator ==(const Vector3& v) const;
	bool operator !=(const Vector3& v) const;

	float operator [](unsigned index) const;
	float& operator [](unsigned index);
};

class Vector4
{
protected:
	mutable float x;
	mutable float y;
	mutable float z;
	mutable float w;

public:
	Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
	~Vector4();

	float length() const;
	float max() const;
	Vector4 normalised() const;

	float getX() const;
	float getY() const;
	float getZ() const;
	float getW() const;

	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setW(float w);
};

class Matrix4
{
private:
	mutable float m[4][4];

public:
	Matrix4();
	~Matrix4();

	Matrix4 initIdentity();
	Matrix4 initTranslation(float x, float y, float z);
	Matrix4 initTranslation(Vector3 v);
	Matrix4 initRotation(float x, float y, float z);
	Matrix4 initScale(float x, float y, float z);
	Matrix4 initPerspective(float fov, float aspectRatio, float zNear, float zFar);
	Matrix4 initOrthographic(float left, float right, float bottom, float top, float near, float far);
	Matrix4 initRotation(const Vector3& forward, const Vector3& up);
	Matrix4 initRotation(const Vector3& forward, const Vector3& up, const Vector3& right);
	Vector3 transform(const Vector3& v) const;
	float getDeterminant() const;
	void setInverse(const Matrix4& m);
	void setComponents(const Vector3& v1, const Vector3& v2, const Vector3& v3);
	Matrix3 toMatrix3() const;

	//From cyclone
	Vector3 getAxisVector(unsigned int index) const;
	Vector3 transformInverse(const Vector3& v) const;
	Vector3 transformDirection(const Vector3& v) const;
	Vector3 transformInverseDirection(const Vector3& v) const;

	float** getM();
	float getAt(int x, int y) const;

	void set(float** m);
	void setAt(int x, int y, float value) const;

	Matrix4 operator *(const Matrix4& m) const;
	Vector3 operator *(const Vector3& v) const;
	float* operator [](int index) const { return m[index]; };
};

class Matrix3
{
private:
	mutable float m[3][3];

public:
	static void pm3(const Matrix3& m, const std::string& msg = "")
	{
		printf("%.4f, %.4f, %.4f\n", m[0][0], m[1][0], m[2][0]);
		printf("%.4f, %.4f, %.4f\n", m[0][1], m[1][1], m[2][1]);
		printf("%.4f, %.4f, %.4f\n", m[0][2], m[1][2], m[2][2]);
	}

	Matrix3();
	~Matrix3();

	Matrix3 initIdentity();

	Vector3 transform(const Vector3& v) const;
	Vector3 transformTranspose(const Vector3& v) const;
	void setInverse(const Matrix3& m);
	Matrix3 inverse() const;
	void setSkewSymmetric(const Vector3& v);
	void setComponents(const Vector3& one, const Vector3& two, const Vector3& three);
	void setTranspose(const Matrix3& m);
	void setInertiaTensorCoeffs(float ix, float iy, float iz, float ixy = 0, float ixz = 0, float iyz = 0);
	void setBlockInteriaTensor(const Vector3& halfSizes, float mass);
	Matrix3 transpose() const;

	float** getM();
	float getAt(int x, int y) const;

	void set(float** m);
	void setAt(int x, int y, float value) const;

	Vector3 operator *(const Vector3& v) const;
	Matrix3 operator *(const Matrix3& m) const;
	void operator +=(const Matrix3& m) const;
	void operator *=(const Matrix3& m);
	void operator *=(float f);

	const float* operator [](int index) const { return m[index]; };
	float& operator [](int index) { return *m[index]; };
};


class Quaternion
{
private:
	float x;
	float y;
	float z;
	float w;

public:
	Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
	Quaternion(const Vector3& axis, float angle);
	Quaternion(Matrix4 rotation);
	~Quaternion();

	float length();
	Quaternion normalised();
	Quaternion conjugate() const;
	float dot(const Quaternion& q) const;
	Quaternion nlerp(Quaternion destination, float lerpFactor, bool shortest);
	Quaternion slerp(Quaternion destination, float lerpFactor, bool shortest);
	Matrix4 toRotationMatrix() const;

	Vector3 getForward() const;
	Vector3 getBackward() const;
	Vector3 getUp() const;
	Vector3 getDown() const;
	Vector3 getLeft() const;
	Vector3 getRight() const;

	float getX() const;
	float getY() const;
	float getZ() const;
	float getW() const;

	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setW(float w);

	Quaternion operator +(const Quaternion& q) const;
	Quaternion operator +(const Vector3& v) const;
	Quaternion operator -(const Quaternion& q) const;
	Quaternion operator *(const Quaternion& q) const;
	Quaternion operator *(const Vector3& v) const;
	Quaternion operator *(float f) const;
	bool operator ==(const Quaternion& q) const;
	bool operator !=(const Quaternion& q) const;
};

#endif