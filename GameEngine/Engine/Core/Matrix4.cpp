#include <cmath>

#include "Math3D.h"

Matrix4::Matrix4()
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			m[i][j] = 0;
		}
	}
}

Matrix4::~Matrix4()
{
}

Matrix4 Matrix4::initIdentity()
{
	m[0][0] = 1;	m[1][0] = 0;	m[2][0] = 0;	m[3][0] = 0;
	m[0][1] = 0;	m[1][1] = 1;	m[2][1] = 0;	m[3][1] = 0;
	m[0][2] = 0;	m[1][2] = 0;	m[2][2] = 1;	m[3][2] = 0;
	m[0][3] = 0;	m[1][3] = 0;	m[2][3] = 0;	m[3][3] = 1;
		
	return *this;
}

Matrix4 Matrix4::initTranslation(float x, float y, float z)
{
	m[0][0] = 1;	m[1][0] = 0;	m[2][0] = 0;	m[3][0] = x;
	m[0][1] = 0;	m[1][1] = 1;	m[2][1] = 0;	m[3][1] = y;
	m[0][2] = 0;	m[1][2] = 0;	m[2][2] = 1;	m[3][2] = z;
	m[0][3] = 0;	m[1][3] = 0;	m[2][3] = 0;	m[3][3] = 1;
	
	return *this;
}

Matrix4 Matrix4::initTranslation(Vector3 v)
{
	return initTranslation(v.getX(), v.getY(), v.getZ());
}


Matrix4 Matrix4::initRotation(float x, float y, float z)
{
	Matrix4 rotX = Matrix4();
	Matrix4 rotY = Matrix4();
	Matrix4 rotZ = Matrix4();

	x = (float)toRadians(x);
	y = (float)toRadians(y);
	z = (float)toRadians(z);

	rotX.m[0][0] = 1;		rotX.m[1][0] = 0;		rotX.m[2][0] = 0;		rotX.m[3][0] = 0;
	rotX.m[0][1] = 0;		rotX.m[1][1] = cos(x);	rotX.m[2][1] = -sin(x);	rotX.m[3][1] = 0;
	rotX.m[0][2] = 0;		rotX.m[1][2] = sin(x);	rotX.m[2][2] = cos(x);	rotX.m[3][2] = 0;
	rotX.m[0][3] = 0;		rotX.m[1][3] = 0;		rotX.m[2][3] = 0;		rotX.m[3][3] = 1;
	
	rotY.m[0][0] = cos(y);	rotY.m[1][0] = 0;		rotY.m[2][0] = -sin(y);	rotY.m[3][0] = 0;
	rotY.m[0][1] = 0;		rotY.m[1][1] = 1;		rotY.m[2][1] = 0;		rotY.m[3][1] = 0;
	rotY.m[0][2] = sin(y);	rotY.m[1][2] = 0;		rotY.m[2][2] = cos(y);	rotY.m[3][2] = 0;
	rotY.m[0][3] = 0;		rotY.m[1][3] = 0;		rotY.m[2][3] = 0;		rotY.m[3][3] = 1;
	
	rotZ.m[0][0] = cos(z);	rotZ.m[1][0] = -sin(z);	rotZ.m[2][0] = 0;		rotZ.m[3][0] = 0;
	rotZ.m[0][1] = sin(z);	rotZ.m[1][1] = cos(z);	rotZ.m[2][1] = 0;		rotZ.m[3][1] = 0;
	rotZ.m[0][2] = 0;		rotZ.m[1][2] = 0;		rotZ.m[2][2] = 1;		rotZ.m[3][2] = 0;
	rotZ.m[0][3] = 0;		rotZ.m[1][3] = 0;		rotZ.m[2][3] = 0;		rotZ.m[3][3] = 1;

	float **m1 = (rotZ * (rotY * rotX)).getM();

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			m[i][j] = m1[i][j];
		}
	}

	return *this;
}

Matrix4 Matrix4::initRotation(const Vector3& forward, const Vector3& up)
{
	Vector3 f = forward.normalised();
	Vector3 r = up.normalised();
	r = r.cross(f);

	Vector3 u = f.cross(r);

	return initRotation(f, u, r);
}

Matrix4 Matrix4::initScale(float x, float y, float z)
{
	m[0][0] = x;	m[1][0] = 0;	m[2][0] = 0;	m[3][0] = 0;
	m[0][1] = 0;	m[1][1] = y;	m[2][1] = 0;	m[3][1] = 0;
	m[0][2] = 0;	m[1][2] = 0;	m[2][2] = z;	m[3][2] = 0;
	m[0][3] = 0;	m[1][3] = 0;	m[2][3] = 0;	m[3][3] = 1;
	
	return *this;
}

Matrix4 Matrix4::initPerspective(float fov, float aspectRatio, float zNear, float zFar)
{
	float tanHalfFov = tanf(fov / 2);
	float zRange = zNear - zFar;

	m[0][0] = 1.0f / (tanHalfFov * aspectRatio);	m[1][0] = 0;					m[2][0] = 0;						m[3][0] = 0;
	m[0][1] = 0;									m[1][1] = 1.0f / tanHalfFov;	m[2][1] = 0;						m[3][1] = 0;
	m[0][2] = 0;									m[1][2] = 0;					m[2][2] = (-zNear - zFar) / zRange;	m[3][2] = 2 * zFar * zNear / zRange;
	m[0][3] = 0;									m[1][3] = 0;					m[2][3] = 1;						m[3][3] = 0;

	return *this;
}

Matrix4 Matrix4::initOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	float width = right - left;
	float height = top - bottom;
	float depth = far - near;
	
	m[0][0] = 2 / width;	m[1][0] = 0;			m[2][0] = 0;			m[3][0] = -(right + left) / width;
	m[0][1] = 0;			m[1][1] = 2 / height;	m[2][1] = 0;			m[3][1] = -(top + bottom) / height;
	m[0][2] = 0;			m[1][2] = 0;			m[2][2] = -2 / depth;	m[3][2] = -(far + near) / depth;
	m[0][3] = 0;			m[1][3] = 0;			m[2][3] = 0;			m[3][3] = 1;
		
	return *this;
}

Matrix4 Matrix4::initRotation(const Vector3& forward, const Vector3& up, const Vector3& right)
{
	Vector3 f = forward;
	Vector3 u = up;
	Vector3 r = right;

	m[0][0] = r.getX();	m[1][0] = r.getY();	m[2][0] = r.getZ();	m[3][0] = 0;
	m[0][1] = u.getX();	m[1][1] = u.getY();	m[2][1] = u.getZ();	m[3][1] = 0;
	m[0][2] = f.getX();	m[1][2] = f.getY();	m[2][2] = f.getZ();	m[3][2] = 0;
	m[0][3] = 0;		m[1][3] = 0;		m[2][3] = 0;		m[3][3] = 1;

	return *this;
}

Vector3 Matrix4::transform(const Vector3& v) const
{
	Vector3 res = Vector3();

	res.setX(res.getX() + m[0][0] * v.getX());
	res.setX(res.getX() + m[1][0] * v.getY());
	res.setX(res.getX() + m[2][0] * v.getZ());

	res.setY(res.getY() + m[0][1] * v.getX());
	res.setY(res.getY() + m[1][1] * v.getY());
	res.setY(res.getY() + m[2][1] * v.getZ());

	res.setZ(res.getZ() + m[0][2] * v.getX());
	res.setZ(res.getZ() + m[1][2] * v.getY());
	res.setZ(res.getZ() + m[2][2] * v.getZ());

	return res;
}

Vector3 Matrix4::getAxisVector(unsigned int index) const
{
	return Vector3(m[index][0], m[index][1], m[index][2]);
}

float** Matrix4::getM()
{
	float** res = (float**)m;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			res[i][j] = m[i][j];
		}
	}

	return res;
}

float Matrix4::getAt(int x, int y) const
{
	return m[x][y];
}

void Matrix4::set(float** m)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			this->m[i][j] = m[i][j];
		}
	}
}

void Matrix4::setAt(int x, int y, float value) const
{
	m[x][y] = value;
}

Matrix4 Matrix4::operator *(const Matrix4& matrix) const
{
	Matrix4 res = Matrix4();

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			for(int k = 0; k < 4; k++)
			{
				res.m[i][j] += this->m[k][j] * matrix.m[i][k];
			}
		}
	}

	return res;
}

Vector3 Matrix4::operator *(const Vector3& v) const
{
	float x = v.getX() * m[0][0] + v.getY() * m[1][0] + v.getZ() * m[2][0] + m[3][0];
	float y = v.getX() * m[0][1] + v.getY() * m[1][1] + v.getZ() * m[2][1] + m[3][1];
	float z = v.getX() * m[0][2] + v.getY() * m[1][2] + v.getZ() * m[2][2] + m[3][2];

	return Vector3(x, y, z);
}