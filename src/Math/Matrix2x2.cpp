/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Math/Matrix2x2.hpp>
//Local
#include <Std++/Math/Vector2/Vector2.inl>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;

//Operators
template <typename ScalarType>
Matrix2x2<ScalarType> Matrix2x2<ScalarType>::operator*(const Matrix2x2& right) const
{
	Matrix2x2 result;

	for(uint8 row = 0; row < 2; row++)
	{
		for(uint8 col = 0; col < 2; col++)
		{
			ScalarType current = 0;
			for(uint8 k = 0; k < 2; k++)
			{
				current += this->operator()(row, k) * right(k, col);
			}

			result(row, col) = current;
		}
	}

	return result;
}

template<typename ScalarType>
Vector2<ScalarType> Matrix2x2<ScalarType>::operator*(const vec2 &v) const
{
	vec2 result;
	for(uint8 row = 0; row < 2; row++)
	{
		ScalarType sum = 0;
		for(uint8 j = 0; j < 2; j++)
			sum += this->columns[j][row] * v[j];

		result[row] = sum;
	}

	return result;
}

//Class functions
template <typename ScalarType>
Matrix2x2<ScalarType> Matrix2x2<ScalarType>::Identity()
{
	Matrix2x2 identity;

	identity(0, 0) = 1;
	identity(1, 1) = 1;

	return identity;
}

template<typename ScalarType>
Matrix2x2<ScalarType> Matrix2x2<ScalarType>::Rotate(ScalarType angle)
{
	ScalarType c = cos(angle);
	ScalarType s = sin(angle);

	mat2 result;
	result(0, 0) = c;
	result(0, 1) = -s;
	result(1, 0) = s;
	result(1, 1) = c;

	return result;
}

namespace StdXX::Math
{
	//Explicit instantiations
	template class Matrix2x2<float32>;
	template class Matrix2x2<float64>;
}