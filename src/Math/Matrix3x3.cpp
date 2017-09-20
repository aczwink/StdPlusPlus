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
//Class Header
#include <ACStdLib/Math/Matrix3x3.hpp>
//Local
#include <ACStdLib/Math/Matrix4x4.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;

//Constructor
Matrix3x3::Matrix3x3(const Matrix4x4 &refMat)
{
	this->columns[0] = refMat[0];
	this->columns[1] = refMat[1];
	this->columns[2] = refMat[2];
}