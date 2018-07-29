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
//Global
#ifdef XPC_FEATURE_SSE3
#include <pmmintrin.h>
#endif
//Local
#include "Vector2.hpp"

#ifdef XPC_FEATURE_SSE2
namespace StdXX
{
	namespace Math
	{
		//Constructors
		template <typename ScalarType>
		inline Vector2<ScalarType>::Vector2()
		{
			this->x = 0;
			this->y = 0;
		}

		template <typename ScalarType>
		inline Vector2<ScalarType>::Vector2(ScalarType x, ScalarType y)
		{
			this->x = x;
			this->y = y;
		}

		//Arithmetic operators
		template <typename ScalarType>
		inline Vector2<ScalarType> Vector2<ScalarType>::operator+(const Vector2<ScalarType> &refRight) const
		{
			return Vector2<ScalarType>(this->x + refRight.x, this->y + refRight.y);
		}

		template <typename ScalarType>
		inline Vector2<ScalarType> Vector2<ScalarType>::operator-(const Vector2<ScalarType> &refRight) const
		{
			return Vector2<ScalarType>(this->x - refRight.x, this->y - refRight.y);
		}

		template <typename ScalarType>
		inline Vector2<ScalarType> Vector2<ScalarType>::operator*(const Vector2<ScalarType> &refRight) const
		{
			return Vector2<ScalarType>(this->x * refRight.x, this->y * refRight.y);
		}

		template <typename ScalarType>
		inline Vector2<ScalarType> &Vector2<ScalarType>::operator*=(ScalarType right)
		{
			this->x *= right;
			this->y *= right;

			return *this;
		}

		template <typename ScalarType>
		inline Vector2<ScalarType> Vector2<ScalarType>::operator/(ScalarType right) const
		{
			return Vector2<ScalarType>(this->x / right, this->y / right);
		}

		//Comparison operators
		template <typename ScalarType>
		inline bool Vector2<ScalarType>::operator==(const Vector2<ScalarType> &other) const
		{
			return (this->x == other.x) && (this->y == other.y);
		}

		template <typename ScalarType>
		inline bool Vector2<ScalarType>::operator!=(const Vector2<ScalarType> &other) const
		{
			return !(*this == other);
		}

		//Inline
		template <typename ScalarType>
		inline ScalarType Vector2<ScalarType>::Dot(const Vector2<ScalarType> &rhs) const
		{
			return this->x * rhs.x + this->y * rhs.y;
		}

		template <typename ScalarType>
		inline ScalarType Vector2<ScalarType>::Length() const
		{
			return sqrt(this->LengthSquared());
		}

		template <typename ScalarType>
		inline ScalarType Vector2<ScalarType>::LengthSquared() const
		{
			return this->x * this->x + this->y * this->y;
		}

		template <typename ScalarType>
		inline Vector2<ScalarType> Vector2<ScalarType>::Normalized() const
		{
			ScalarType length = this->Length();
			return vec2(this->x / length, this->y / length);
		}
	}
}
#endif