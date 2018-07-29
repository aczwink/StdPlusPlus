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
#include <Std++/Math/Vector3.hpp>
//Local
#include <Std++/Math/Vector4/Vector4.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;

//Constructors
template <typename ScalarType>
Vector3<ScalarType>::Vector3(const Vector4<ScalarType> &refV)
{
	this->x = refV.x;
	this->y = refV.y;
	this->z = refV.z;
}

//Explicit instantiations
template class STDPLUSPLUS_API StdXX::Math::Vector3<float32>;
template class STDPLUSPLUS_API StdXX::Math::Vector3<float64>;