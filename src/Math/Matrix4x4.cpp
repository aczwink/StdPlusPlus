/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of Std++.
 *
 * Std++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Std++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Std++.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <Std++/Math/Matrix4x4.hpp>
//Local
#include <Std++/Math/Matrix3x3.hpp>
#include <Std++CPUOpt.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;

//Constructor
template <typename ScalarType>
Matrix4x4<ScalarType>::Matrix4x4(const Matrix3x3<ScalarType> &refMatrix)
{
	this->columns[0] = vec4(refMatrix[0], 0);
	this->columns[1] = vec4(refMatrix[1], 0);
	this->columns[2] = vec4(refMatrix[2], 0);
	this->columns[3].w = 1;
}

//Operators
template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::operator*(float32 right) const
{
	Matrix4x4 tmp;

	tmp = *this;
	tmp.columns[0] *= right;
	tmp.columns[1] *= right;
	tmp.columns[2] *= right;
	tmp.columns[3] *= right;

	return tmp;
}

template <typename ScalarType>
vec4f32 Matrix4x4<ScalarType>::operator*(const vec4f32 &refRight) const
{
	float64 current;
	vec4f32 result;

	for(uint8 row = 0; row < 4; row++)
	{
		current = 0;
		for(uint8 j = 0; j < 4; j++)
		{
			current += this->columns[j][row] * refRight[j];
		}

		result[row] = (float32)current;
	}

	return result;
}

template <typename ScalarType>
Vector4<ScalarType> Matrix4x4<ScalarType>::operator*(const Vector4<ScalarType> &rhs) const
{
	float64 current;
	vec4 result;

	for(uint8 row = 0; row < 4; row++)
	{
		current = 0;
		for(uint8 j = 0; j < 4; j++)
		{
			current += this->operator()(row, j) * rhs[j];
		}

		result[row] = (ScalarType)current;
	}

	return result;
}

template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::operator*(const Matrix4x4 &refRight) const
{
	float64 current;
	Matrix4x4 result;

	for(uint8 row = 0; row < 4; row++)
	{
		for(uint8 col = 0; col < 4; col++)
		{
			current = 0;
			for(uint8 k = 0; k < 4; k++)
			{
				current += this->operator()(row, k) * refRight(k, col);
			}

			result(row, col) = (float32)current;
		}
	}

	return result;
}

//Public methods
template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::Inverse() const
{
	/**
	 * This method was copied and changed such that it matches this class.
	 * This is the 1/det explicit method
	 * TODO: implement this myself
	 */

	float32 Coef00 = this->columns[2][2] * this->columns[3][3] - this->columns[3][2] * this->columns[2][3];
	float32 Coef02 = this->columns[1][2] * this->columns[3][3] - this->columns[3][2] * this->columns[1][3];
	float32 Coef03 = this->columns[1][2] * this->columns[2][3] - this->columns[2][2] * this->columns[1][3];

	float32 Coef04 = this->columns[2][1] * this->columns[3][3] - this->columns[3][1] * this->columns[2][3];
	float32 Coef06 = this->columns[1][1] * this->columns[3][3] - this->columns[3][1] * this->columns[1][3];
	float32 Coef07 = this->columns[1][1] * this->columns[2][3] - this->columns[2][1] * this->columns[1][3];

	float32 Coef08 = this->columns[2][1] * this->columns[3][2] - this->columns[3][1] * this->columns[2][2];
	float32 Coef10 = this->columns[1][1] * this->columns[3][2] - this->columns[3][1] * this->columns[1][2];
	float32 Coef11 = this->columns[1][1] * this->columns[2][2] - this->columns[2][1] * this->columns[1][2];

	float32 Coef12 = this->columns[2][0] * this->columns[3][3] - this->columns[3][0] * this->columns[2][3];
	float32 Coef14 = this->columns[1][0] * this->columns[3][3] - this->columns[3][0] * this->columns[1][3];
	float32 Coef15 = this->columns[1][0] * this->columns[2][3] - this->columns[2][0] * this->columns[1][3];

	float32 Coef16 = this->columns[2][0] * this->columns[3][2] - this->columns[3][0] * this->columns[2][2];
	float32 Coef18 = this->columns[1][0] * this->columns[3][2] - this->columns[3][0] * this->columns[1][2];
	float32 Coef19 = this->columns[1][0] * this->columns[2][2] - this->columns[2][0] * this->columns[1][2];

	float32 Coef20 = this->columns[2][0] * this->columns[3][1] - this->columns[3][0] * this->columns[2][1];
	float32 Coef22 = this->columns[1][0] * this->columns[3][1] - this->columns[3][0] * this->columns[1][1];
	float32 Coef23 = this->columns[1][0] * this->columns[2][1] - this->columns[2][0] * this->columns[1][1];

	vec4 Fac0(Coef00, Coef00, Coef02, Coef03);
	vec4 Fac1(Coef04, Coef04, Coef06, Coef07);
	vec4 Fac2(Coef08, Coef08, Coef10, Coef11);
	vec4 Fac3(Coef12, Coef12, Coef14, Coef15);
	vec4 Fac4(Coef16, Coef16, Coef18, Coef19);
	vec4 Fac5(Coef20, Coef20, Coef22, Coef23);

	vec4 Vec0(this->columns[1][0], this->columns[0][0], this->columns[0][0], this->columns[0][0]);
	vec4 Vec1(this->columns[1][1], this->columns[0][1], this->columns[0][1], this->columns[0][1]);
	vec4 Vec2(this->columns[1][2], this->columns[0][2], this->columns[0][2], this->columns[0][2]);
	vec4 Vec3(this->columns[1][3], this->columns[0][3], this->columns[0][3], this->columns[0][3]);

	vec4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
	vec4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
	vec4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
	vec4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

	vec4 SignA(+1, -1, +1, -1);
	vec4 SignB(-1, +1, -1, +1);
	Matrix4x4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

	vec4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

	vec4 Dot0(this->columns[0] * Row0);
	float32 Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

	float32 OneOverDeterminant = 1 / Dot1;

	return Inverse * OneOverDeterminant;
}

template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::Transpose() const
{
	Matrix4x4 result;

	for(uint8 row = 0; row < 4; row++)
	{
		for(uint8 col = 0; col < 4; col++)
		{
			result(col, row) = (*this)(row, col);
		}
	}

	return result;
}

//Class functions
template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::Identity()
{
	Matrix4x4 identity;

	identity(0, 0) = 1;
	identity(1, 1) = 1;
	identity(2, 2) = 1;
	identity(3, 3) = 1;

	return identity;
}

template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::OrthographicRH(float32 left, float32 right, float32 bottom, float32 top, float32 zNear, float32 zFar)
{
	Matrix4x4 ortho;

	ortho(0, 0) = 2 / (right - left);
	ortho(0, 3) = - (right + left) / (right - left);
	ortho(1, 1) = 2 / (top - bottom);
	ortho(1, 3) = -(top + bottom) / (top - bottom);
	ortho(2, 2) = -2 / (zFar - zNear);
	ortho(2, 3) = -(zFar + zNear) / (zFar - zNear);
	ortho(3, 3) = 1;

	return ortho;
}

template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::PerspectiveRH(const Degree<ScalarType> &refFovY, float32 aspectRatio, float32 zNear, float32 zFar)
{
	float32 tanHalfFovY, range;
	Matrix4x4 perspective;

	tanHalfFovY = tanf(Radian<ScalarType>(refFovY).value / 2);

	range = (zFar - zNear);

	perspective(0, 0) = 1 / (aspectRatio * tanHalfFovY);
	perspective(1, 1) = 1 / tanHalfFovY;
	perspective(2, 2) = -(zFar + zNear) / range;
	perspective(2, 3) = -2 * zFar * zNear / range;
	perspective(3, 2) = -1;

	return perspective;
}

template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::RotationX(const Radian<ScalarType> &refAngle)
{
	Matrix4x4 rotate;

	rotate(0, 0) = 1;
	rotate(1, 1) = cosf(refAngle.value);
	rotate(2, 1) = sinf(refAngle.value);
	rotate(1, 2) = -rotate(2, 1);
	rotate(2, 2) = rotate(1, 1);
	rotate(3, 3) = 1;

	return rotate;
}

template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::RotationY(const Radian<ScalarType> &refAngle)
{
	Matrix4x4 rotate;

	rotate(0, 0) = cosf(refAngle.value);
	rotate(0, 2) = sinf(refAngle.value);
	rotate(1, 1) = 1;
	rotate(2, 0) = -rotate(0, 2);
	rotate(2, 2) = rotate(0, 0);
	rotate(3, 3) = 1;

	return rotate;
}

template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::RotationYawPitchRoll(const Radian<ScalarType> &refYaw, const Radian<ScalarType> &refPitch, const Radian<ScalarType> &refRoll)
{
	return Matrix4x4::RotationX(refPitch) * Matrix4x4::RotationY(refYaw) * Matrix4x4::RotationZ(refRoll);
}

template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::RotationZ(const Radian<ScalarType> &refAngle)
{
	Matrix4x4 rotate;

	rotate(0, 0) = cosf(refAngle.value);
	rotate(1, 0) = sinf(refAngle.value);
	rotate(0, 1) = -rotate(1, 0);
	rotate(1, 1) = rotate(0, 0);
	rotate(2, 2) = 1;
	rotate(3, 3) = 1;

	return rotate;
}

template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::Scale(const vec3 &s)
{
	Matrix4x4 scale;

	scale(0, 0) = s.x;
	scale(1, 1) = s.y;
	scale(2, 2) = s.z;
	scale(3, 3) = 1;

	return scale;
}

template <typename ScalarType>
Matrix4x4<ScalarType> Matrix4x4<ScalarType>::Translation(const vec3 &t)
{
	Matrix4x4 translate;

	translate(0, 0) = 1;
	translate(1, 1) = 1;
	translate(2, 2) = 1;
	translate[3] = vec4(t, 1);

	return translate;
}

namespace StdXX
{
	namespace Math
	{
		//Explicit instantiations
		template class Matrix4x4<float32>;
	}
}
