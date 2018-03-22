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
#include <Std++/Math/Matrix2x2.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Math;

//Operators
template <typename ScalarType>
Matrix2x2<ScalarType> Matrix2x2<ScalarType>::operator*(const Matrix2x2 &refRight) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return Matrix2x2();
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

template <typename ScalarType>
Matrix2x2<ScalarType> Matrix2x2<ScalarType>::Scale(float32 scaleX, float32 scaleY)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return Matrix2x2();
}

//Explicit instantiations
template class Matrix2x2<float32>;