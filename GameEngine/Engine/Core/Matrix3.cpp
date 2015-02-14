#include "Math3D.h"

Matrix3::Matrix3()
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			m[i][j] = 0;
		}
	}
}

Matrix3::~Matrix3()
{

}

Matrix3 Matrix3::initIdentity()
{
	m[0][0] = 1;	m[1][0] = 0;	m[2][0] = 0;
	m[0][1] = 0;	m[1][1] = 1;	m[2][1] = 0;
	m[0][2] = 0;	m[1][2] = 0;	m[2][2] = 1;

	return *this;
}

Vector3 Matrix3::transform(const Vector3& v) const
{
	return (*this) * v;
}

Vector3 Matrix3::transformTranspose(const Vector3& v) const
{
	float x = v.getX() * m[0][0] + v.getY() * m[0][1] + v.getZ() * m[0][2];
	float y = v.getX() * m[1][0] + v.getY() * m[1][1] + v.getZ() * m[1][2];
	float z = v.getX() * m[2][0] + v.getY() * m[2][1] + v.getZ() * m[2][2];

	return Vector3(x, y, z);
}

void Matrix3::setInverse(const Matrix3& m)
{
	float t4 = m.m[0][0] * m.m[1][1];
	float t6 = m.m[0][0] * m.m[2][1];
	float t8 = m.m[1][0] * m.m[0][1];
	float t10 = m.m[2][0] * m.m[0][1];
	float t12 = m.m[1][0] * m.m[0][2];
	float t14 = m.m[2][0] * m.m[0][2];

	float t16 = (t4 * m.m[2][2] - t6 *m.m[1][2] -
		t8 * m.m[2][2] + t10 * m.m[1][2] +
		t12 * m.m[2][1] - t14 * m.m[1][1]);

	if(t16 == 0.0f)
	{
		return;
	}

	float t17 = 1.0f / t16;

	setAt(0, 0, (m.m[1][1] * m.m[2][2] - m.m[2][1] * m.m[1][2]) * t17);
	setAt(1, 0, -(m.m[1][0] * m.m[2][2] - m.m[2][0] * m.m[1][2]) * t17);
	setAt(2, 0, (m.m[1][0] * m.m[2][1] - m.m[2][0] * m.m[1][1]) * t17);
	setAt(0, 1, -(m.m[0][1] * m.m[2][2] - m.m[2][1] * m.m[0][2]) * t17);
	setAt(1, 1, (m.m[0][0] * m.m[2][2] - t14) * t17);
	setAt(2, 1, -(t6 - t10) * t17);
	setAt(0, 2, (m.m[0][1] * m.m[1][2] - m.m[1][1] * m.m[0][2]) * t17);
	setAt(1, 2, -(m.m[0][0] * m.m[1][2] - t12) * t17);
	setAt(2, 2, (t4 - t8) * t17);
}

Matrix3 Matrix3::inverse() const
{
	Matrix3 result;
	result.setInverse(*this);
	return result;
}

void Matrix3::setSkewSymmetric(const Vector3& v)
{
	m[0][0] = 0;
	m[1][1] = 0;
	m[2][2] = 0;

	m[1][0] = -v.getZ();
	m[2][0] = v.getY();
	m[0][1] = v.getZ();
	m[2][1] = -v.getX();
	m[0][2] = -v.getY();
	m[1][2] = v.getX();
}

void Matrix3::setComponents(const Vector3& one, const Vector3& two, const Vector3& three)
{
	m[0][0] = one.getX();
	m[1][0] = two.getX();
	m[2][0] = three.getX();
	m[0][1] = one.getY();
	m[1][1] = two.getY();
	m[2][1] = three.getY();
	m[0][2] = one.getZ();
	m[1][2] = two.getZ();
	m[2][2] = three.getZ();
}

void Matrix3::setTranspose(const Matrix3& matrix)
{
	m[0][0] = matrix.m[0][0];
	m[1][0] = matrix.m[1][0];
	m[2][0] = matrix.m[2][0];
	m[0][1] = matrix.m[0][1];
	m[1][1] = matrix.m[1][1];
	m[2][1] = matrix.m[2][1];
	m[0][2] = matrix.m[0][2];
	m[1][2] = matrix.m[1][2];
	m[2][2] = matrix.m[2][2];
}

void Matrix3::setBlockInteriaTensor(const Vector3& halfSizes, float mass)
{
	Vector3 squares = halfSizes * halfSizes;
	setInertiaTensorCoeffs(0.3f * mass * (squares.getY() * squares.getZ()),
		0.3f * mass * (squares.getX() + squares.getZ()),
		0.3f * mass * (squares.getX() + squares.getY()));
}

void Matrix3::setInertiaTensorCoeffs(float ix, float iy, float iz, float ixy, float ixz, float iyz)
{
	m[0][0] = ix;	m[1][0] = -ixy;	m[2][0] = -ixz;
	m[0][1] = -ixy; m[1][1] = iy;	m[2][1] = -iyz;
	m[0][2] = -ixz; m[1][2] = -iyz; m[2][2] = iz;
}

Matrix3 Matrix3::transpose() const
{
	Matrix3 result;
	result.setTranspose(*this);
	return result;
}

float** Matrix3::getM()
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

float Matrix3::getAt(int x, int y) const
{
	return m[x][y];
}

void Matrix3::set(float** m)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			this->m[i][j] = m[i][j];
		}
	}
}

void Matrix3::setAt(int x, int y, float value) const
{
	m[x][y] = value;
}

Vector3 Matrix3::operator *(const Vector3& v) const
{
	float x = v.getX() * m[0][0] + v.getY() * m[1][0] + v.getZ() * m[2][0];
	float y = v.getX() * m[0][1] + v.getY() * m[1][1] + v.getZ() * m[2][1];
	float z = v.getX() * m[0][2] + v.getY() * m[1][2] + v.getZ() * m[2][2];

	return Vector3(x, y, z);
}

Matrix3 Matrix3::operator *(const Matrix3& matrix) const
{
	Matrix3 res = Matrix3();

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

void Matrix3::operator +=(const Matrix3& matrix) const
{
	m[0][0] += matrix.m[0][0];
	m[1][0] += matrix.m[1][0];
	m[2][0] += matrix.m[2][0];
	m[0][1] += matrix.m[0][1];
	m[1][1] += matrix.m[1][1];
	m[2][1] += matrix.m[2][1];
	m[0][2] += matrix.m[0][2];
	m[1][2] += matrix.m[1][2];
	m[2][2] += matrix.m[2][2];
}

void Matrix3::operator *=(const Matrix3& matrix)
{
	float t1 = m[0][0] * matrix.m[0][0] + m[1][0] * matrix.m[0][1] + m[2][0] * matrix.m[0][2];
	float t2 = m[0][0] * matrix.m[0][1] + m[1][0] * matrix.m[1][1] + m[2][0] * matrix.m[1][2];
	float t3 = m[0][0] * matrix.m[0][2] + m[1][0] * matrix.m[2][1] + m[2][0] * matrix.m[2][2];

	m[0][0] = t1;
	m[1][0] = t2;
	m[2][0] = t3;

	t1 = m[0][1] * matrix.m[0][0] + m[1][1] * matrix.m[0][1] + m[2][1] * matrix.m[0][2];
	t2 = m[0][1] * matrix.m[0][1] + m[1][1] * matrix.m[1][1] + m[2][1] * matrix.m[1][2];
	t3 = m[0][1] * matrix.m[0][2] + m[1][1] * matrix.m[2][1] + m[2][1] * matrix.m[2][2];

	m[0][1] = t1;
	m[1][1] = t2;
	m[2][1] = t3;

	t1 = m[0][2] * matrix.m[0][0] + m[1][2] * matrix.m[0][1] + m[2][2] * matrix.m[0][2];
	t2 = m[0][2] * matrix.m[0][1] + m[1][2] * matrix.m[1][1] + m[2][2] * matrix.m[1][2];
	t3 = m[0][2] * matrix.m[0][2] + m[1][2] * matrix.m[2][1] + m[2][2] * matrix.m[2][2];

	m[0][2] = t1;
	m[1][2] = t2;
	m[2][2] = t3;
}

void Matrix3::operator *=(float f)
{
	m[0][0] *= f;
	m[1][0] *= f;
	m[2][0] *= f;
	m[0][1] *= f;
	m[1][1] *= f;
	m[2][1] *= f;
	m[0][2] *= f;
	m[1][2] *= f;
	m[2][2] *= f;
}