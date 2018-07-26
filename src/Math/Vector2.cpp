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
#include <Std++/Math/Vector2.hpp>
//Local
#include <Std++CPUOpt.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;

//Constructor
template <typename ScalarType>
Vector2<ScalarType>::Vector2(const vec2f64 &refV)
{
	this->x = (float32)refV.x;
	this->y = (float32)refV.y;
}

//Explicit instantiations
template class StdXX::Math::Vector2<float32>;