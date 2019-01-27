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
#include "Std++/Definitions.h"
#include "Std++/Debug.hpp"
#include <Std++/Math/Vector3.hpp>
#include <Std++/Type.hpp>


#include "Vector4Base_scalar.inl"
#include "Vector4Base_SSE.inl"

namespace StdXX
{
    namespace Math
    {
		template<typename ScalarType>
        class alignas(4*sizeof(ScalarType)) Vector4 : public _stdxx_::Vector4Base<ScalarType>
        {
			typedef Vector4<ScalarType> vec4;
        public:
            //Constructors
            inline Vector4()
            {
                this->x = 0;
                this->y = 0;
                this->z = 0;
                this->w = 0;
            }

            inline Vector4(float32 x, float32 y, float32 z, float32 w)
            {
                this->x = x;
                this->y = y;
                this->z = z;
                this->w = w;
            }

            Vector4(const Vector3<ScalarType> &refXYZ, float32 w);

#ifdef XPC_FEATURE_SSE2
			template <class T = ScalarType>
			inline Vector4(typename EnableIf<IsSameType<T, float32>::value, __m128>::type mmValue)
			{
				this->mmValue = mmValue;
			}
#endif

            //Operators
            inline float32 &operator[](uint8 idx)
            {
                ASSERT(idx < 4, "Column must be < 4");

                return this->e[idx];
            }

            inline const float32 &operator[](uint8 idx) const
            {
                ASSERT(idx < 4, "Column must be < 4");

                return this->e[idx];
            }

            inline Vector4 operator+(const Vector4 &refRight) const
            {
                return Vector4(this->x + refRight.x, this->y + refRight.y, this->z + refRight.z, this->w + refRight.w);
            }

            inline vec4& operator+=(const vec4& rhs)
			{
				*this = (*this) + rhs;
				return *this;
			}

            inline Vector4 operator-(const Vector4 &refRight) const
            {
                return Vector4(this->x - refRight.x, this->y - refRight.y, this->z - refRight.z, this->w - refRight.w);
            }

            inline vec4 operator*(ScalarType scalar) const;

            inline Vector4 operator*(const Vector4 &refRight) const
            {
                return Vector4(this->x * refRight.x, this->y * refRight.y, this->z * refRight.z, this->w * refRight.w);
            }

			inline vec4 operator/(ScalarType scalar) const;

			inline Vector4 operator/(const Vector4 &refRight) const
			{
				return Vector4(this->x / refRight.x, this->y / refRight.y, this->z / refRight.z, this->w / refRight.w);
			}

            inline Vector4 &operator*=(float32 right)
            {
                this->x *= right;
                this->y *= right;
                this->z *= right;
                this->w *= right;

                return *this;
            }

            //Inline
            inline Vector4<ScalarType> Cross(const Vector4<ScalarType> &rhs) const;
			inline ScalarType Dot(const Vector4<ScalarType> &rhs) const;
			inline ScalarType Length() const;
			inline vec4 Normalized() const;
        };

        typedef Vector4<float32> Vector4S;
        typedef Vector4<float64> Vector4D;

        //Right-hand side operators
		template <typename ScalarType>
		inline Vector4<ScalarType> operator*(ScalarType lhs, const Vector4<ScalarType>& rhs)
		{
			return rhs * lhs;
		}
    }
}