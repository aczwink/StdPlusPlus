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
#include <Std++/Math/Vector4.hpp>
//Local
#include <Std++/Math/Vector3.hpp>

//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Math;

//Constructors
template <typename ScalarType>
Vector4<ScalarType>::Vector4(const Vector3<ScalarType> &refXYZ, float32 w)
{
	this->x = refXYZ.x;
	this->y = refXYZ.y;
	this->z = refXYZ.z;
	this->w = w;
}

//Explicit instantiations
template class StdPlusPlus::Math::Vector4<float32>;