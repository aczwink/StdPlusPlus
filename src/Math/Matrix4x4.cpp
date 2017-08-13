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
#include <ACStdLib/Math/Matrix4x4.hpp>
//Local
#include <ACStdLib/Math/Matrix3x3.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;

//Constructor
Matrix4x4::Matrix4x4(const Matrix3x3 &refMatrix)
{
	this->columns[0] = Vector4(refMatrix[0], 0);
	this->columns[1] = Vector4(refMatrix[1], 0);
	this->columns[2] = Vector4(refMatrix[2], 0);
	this->columns[3].w = 1;
}

//Operators
Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &refRight) const
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