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
#pragma once
//Local
#include "Std++/__Globaldependencies.h" //needed for sqrt
#include "Std++/Definitions.h"
#include "Std++/Debug.hpp"
#include <Std++/Type.hpp>

#include "Vector2Base_scalar.inl"
#include "Vector2Base_SSE.inl"

namespace StdXX
{
    namespace Math
    {
        template <typename ScalarType>
        class alignas(2*sizeof(ScalarType)) Vector2 : public _stdxx_::Vector2Base<ScalarType>
        {
			typedef Vector2<ScalarType> vec2;
        public:
            //Constructors
            inline Vector2();
            inline Vector2(ScalarType x, ScalarType y);

#ifdef XPC_FEATURE_SSE2
			template <class T = ScalarType>
			inline Vector2(typename EnableIf<IsSameType<T, float64>::value, __m128d>::type mmValue)
			{
				this->mmValue = mmValue;
			}
#endif

            //Access operators
			inline ScalarType &operator[](uint8 idx)
			{
				ASSERT(idx < 2, "Column must be < 2");

				return this->e[idx];
			}

			inline const ScalarType &operator[](uint8 idx) const
			{
				ASSERT(idx < 2, "Column must be < 2");

				return this->e[idx];
			}

			//Arithmetic operators
			inline Vector2 operator-()
			{
				return Vector2(-this->x, -this->y);
			}
			inline Vector2 operator+(const Vector2 &refRight) const;
			inline Vector2& operator+=(const Vector2& rhs)
			{
				*this = *this + rhs;
				return *this;
			}

			inline Vector2 operator-(const Vector2 &refRight) const;
			inline Vector2 operator*(ScalarType rhs) const;
			inline Vector2 operator*(const Vector2 &refRight) const;
			inline Vector2 &operator*=(ScalarType right);
			inline Vector2 operator/(ScalarType right) const;

			//Comparison operators
			inline bool operator==(const vec2 &other) const;
			inline bool operator!=(const vec2 &other) const;

			//Inline
			inline ScalarType Cross2D(const vec2 &rhs) const
			{
				return this->x * rhs.y - this->y * rhs.x;
			}

			inline ScalarType Dot(const vec2 &rhs) const;
			inline ScalarType Length() const;
			inline ScalarType LengthSquared() const;

            inline vec2 &Normalize()
            {
                *this = this->Normalized();
                return *this;
            }

            inline vec2 Normalized() const;

			inline vec2 PerpendicularDirection() const
			{
				return vec2(this->y, -this->x);
			}

			inline vec2 Rotate(ScalarType angle) const;
        };

        //Binary operators
		template <typename ScalarType>
        inline Vector2<ScalarType> operator*(ScalarType lhs, const Vector2<ScalarType>& rhs)
        {
			return rhs * lhs;
        }

		typedef Vector2<float32> Vector2S;
		typedef Vector2<float64> Vector2D;
    }
}