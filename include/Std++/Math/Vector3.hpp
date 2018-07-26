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
#pragma once
//Local
#include "../__Globaldependencies.h" //needed for sqrt
#include "../Definitions.h"
#include "Std++/Debug.hpp"

namespace StdXX
{
    namespace Math
    {
		//Forward declarations
		template <typename ScalarType>
		class Vector4;

        template <typename ScalarType>
		class STDPLUSPLUS_API Vector3
        {
			typedef Vector3<ScalarType> vec3;
        public:
            //Members
            union
            {
                struct
                {
					ScalarType x;
					ScalarType y;
					ScalarType z;
                };
				ScalarType e[3];
            };

            //Constructors
            inline Vector3()
            {
                this->x = 0;
                this->y = 0;
                this->z = 0;
            }

            inline Vector3(ScalarType x, ScalarType y, ScalarType z)
            {
                this->x = x;
                this->y = y;
                this->z = z;
            }

			template<typename OtherScalarType>
			inline Vector3(const Vector3<OtherScalarType> &other) : x(other.x), y(other.y), z(other.z)
			{
			}

            Vector3(const Vector4<ScalarType> &refV);

            //Operators
            inline ScalarType &operator[](uint8 idx)
            {
                ASSERT(idx < 3, "Column must be < 3");

                return this->e[idx];
            }

            inline const ScalarType &operator[](uint8 idx) const
            {
                ASSERT(idx < 3, "Column must be < 3");

                return this->e[idx];
            }

            inline Vector3 &operator=(const Vector3 &refOther)
            {
                this->x = refOther.x;
                this->y = refOther.y;
                this->z = refOther.z;

                return *this;
            }

			inline Vector3 operator+(const vec3 &rhs) const
			{
				return vec3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
			}

			inline Vector3 &operator+=(const Vector3 &refOther)
			{
				this->x += refOther.x;
				this->y += refOther.y;
				this->z += refOther.z;

				return *this;
			}

			inline Vector3 operator-(const vec3 &rhs) const
			{
				return vec3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
			}

			inline Vector3 &operator-=(const Vector3 &refOther)
			{
				this->x -= refOther.x;
				this->y -= refOther.y;
				this->z -= refOther.z;

				return *this;
			}

			inline Vector3 operator*(ScalarType scalar) const
			{
				return vec3(this->x * scalar, this->y * scalar, this->z * scalar);
			}

			inline Vector3 operator/(ScalarType scalar) const
			{
				return vec3(this->x / scalar, this->y / scalar, this->z / scalar);
			}

			inline Vector3 &operator/=(ScalarType scalar)
			{
				this->x /= scalar;
				this->y /= scalar;
				this->z /= scalar;

				return *this;
			}

            inline Vector3 operator-() const
            {
                return Vector3(-this->x, -this->y, -this->z);
            }

			inline bool operator==(const vec3 &rhs) const
			{
				return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
			}

            //Inline
			inline Vector3 Cross(const Vector3 &rhs) const
			{
				return vec3(
					this->y * rhs.z - this->z * rhs.y,
					this->z * rhs.x - this->x * rhs.z,
					this->x * rhs.y - this->y * rhs.x
				);
			}

			inline ScalarType Dot(const vec3 &rhs) const
			{
				return ScalarType(this->x * rhs.x + this->y * rhs.y + this->z * rhs.z);
			}

            inline ScalarType Length() const
            {
                return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
            }

			inline vec3 &Normalize()
			{
				*this = this->Normalized();
				return *this;
			}

            inline vec3 Normalized() const
            {
				ScalarType length = this->Length();
				return vec3(this->x / length, this->y / length, this->z / length);
            }
        };

        //Class operators
		template <typename ScalarType>
        inline Vector3<ScalarType> operator*(ScalarType scalar, const Vector3<ScalarType> &refRight)
        {
            return Vector3<ScalarType>(scalar * refRight.x, scalar * refRight.y, scalar * refRight.z);
        }

		typedef Vector3<float32> Vector3s;
    }
}