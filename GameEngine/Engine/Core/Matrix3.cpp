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