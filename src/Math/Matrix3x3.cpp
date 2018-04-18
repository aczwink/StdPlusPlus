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
//Class Header
#include <Std++/Math/Matrix3x3.hpp>
//Local
#include <Std++/Math/Matrix4x4.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Math;

//Constructor
template <typename ScalarType>
Matrix3x3<ScalarType>::Matrix3x3(const Matrix4x4<ScalarType> &refMat)
{
	this->columns[0] = refMat[0];
	this->columns[1] = refMat[1];
	this->columns[2] = refMat[2];
}

//Operators
template<typename ScalarType>
Vector3<ScalarType> Matrix3x3<ScalarType>::operator*(const vec3 &v) const
{
	vec3 result;
	for(uint8 row = 0; row < 3; row++)
	{
		ScalarType sum = 0;
		for(uint8 j = 0; j < 3; j++)
			sum += this->columns[j][row] * v[j];

		result[row] = sum;
	}

	return result;
}

//Class functions
template<typename ScalarType>
Matrix3x3<ScalarType> Matrix3x3<ScalarType>::Rotation(const Radian<ScalarType> &angle, const vec3 &axis)
{
	//Based on https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle

	vec3 an = axis.Normalized();
	ScalarType c = cos(angle.value);
	ScalarType s = sin(angle.value);
	vec3 cv = (1 - c) * an;
	vec3 sv = s * an;

	mat3 result;
	//x
	result[0] = an * cv.x;
	result(0, 0) += c;
	result(1, 0) += sv.z;
	result(2, 0) -= sv.y;
	//y
	result[1] = an * cv.y;
	result(0, 1) -= sv.z;
	result(1, 1) += c;
	result(2, 1) += sv.x;
	//z
	result[2] = an * cv.z;
	result(0, 2) += sv.y;
	result(1, 2) -= sv.x;
	result(2, 2) += c;

	return result;
}

//Explicit instantiations
template class Matrix3x3<float64>;