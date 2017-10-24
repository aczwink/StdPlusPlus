/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <ACStdLib/Math/Matrix2x2.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;

//Operators
Matrix2x2 Matrix2x2::operator*(const Matrix2x2 &refRight) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return Matrix2x2();
}

//Class functions
Matrix2x2 Matrix2x2::Identity()
{
	Matrix2x2 identity;

	identity(0, 0) = 1;
	identity(1, 1) = 1;

	return identity;
}

Matrix2x2 Matrix2x2::Scale(float32 scaleX, float32 scaleY)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return Matrix2x2();
}