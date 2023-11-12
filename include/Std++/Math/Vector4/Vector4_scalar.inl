/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
//Local
#include "Vector4.hpp"

namespace StdXX::Math
{
	//Operators
	template<typename ScalarType>
	inline Vector4<ScalarType> Vector4<ScalarType>::operator*(ScalarType scalar) const
	{
		return {this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar};
	}

	template<typename ScalarType>
	inline Vector4<ScalarType> Vector4<ScalarType>::operator/(ScalarType scalar) const
	{
		return {this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar};
	}

	//Inline
	template<typename ScalarType>
	inline Vector4<ScalarType> Vector4<ScalarType>::Cross(const Vector4<ScalarType> &rhs) const
	{
		auto s1 = this->e[1] * rhs.e[2] - this->e[2] * rhs.e[1];
		auto s2 = this->e[2] * rhs.e[0] - this->e[0] * rhs.e[2];
		auto s3 = this->e[0] * rhs.e[1] - this->e[1] * rhs.e[0];

		return Vector4<ScalarType>(s1, s2, s3, 1);
	}

	template<typename ScalarType>
	inline ScalarType Vector4<ScalarType>::Dot(const Vector4<ScalarType> &rhs) const
	{
		return (this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w);
	}
}