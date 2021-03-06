/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Math/Degree.hpp>
//Local
#include <Std++/Math/Radian.hpp>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;

//Constructor
template <typename ScalarType>
Degree<ScalarType>::Degree(const Radian<ScalarType> &refRadian)
{
	this->value = refRadian.value * 180.0 / PI;
}

namespace StdXX
{
	namespace Math
	{
		//Explicit instantiations
		template class STDPLUSPLUS_API Degree<float32>;
		template class STDPLUSPLUS_API Degree<float64>;
	}
}