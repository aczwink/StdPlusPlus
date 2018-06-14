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

namespace StdPlusPlus
{
    //Move declarations
    class vec2f64;

    namespace Math
    {
        template <typename ScalarType>
        class Vector2
        {
			typedef Vector2<ScalarType> vec2;
        public:
            //Members
            union
            {
                struct
                {
					ScalarType x;
					ScalarType y;
                };
				ScalarType e[2];
            };

            //Constructors
            inline Vector2()
            {
                this->x = 0;
                this->y = 0;
            }

            inline Vector2(ScalarType x, ScalarType y)
            {
                this->x = x;
                this->y = y;
            }

            Vector2(const vec2f64 &refV);

            //Operators
            inline bool operator==(const vec2 &other) const
			{
				return (this->x == other.x) && (this->y == other.y);
			}

			inline bool operator!=(const vec2 &other) const
			{
				return !(*this == other);
			}

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

            inline Vector2 operator-()
            {
                return Vector2(-this->x, -this->y);
            }

            //Binary operators
            inline Vector2 operator+(const Vector2 &refRight) const
            {
                return Vector2(this->x + refRight.x, this->y + refRight.y);
            }

            inline Vector2 operator-(const Vector2 &refRight) const
            {
                return Vector2(this->x - refRight.x, this->y - refRight.y);
            }

            inline Vector2 operator*(ScalarType right) const
            {
                return Vector2(this->x * right, this->y * right);
            }

            inline Vector2 operator*(const Vector2 &refRight) const
            {
                return Vector2(this->x * refRight.x, this->y * refRight.y);
            }

            inline Vector2 &operator*=(ScalarType right)
            {
                this->x *= right;
                this->y *= right;

                return *this;
            }

            inline Vector2 operator/(ScalarType right) const
            {
                return Vector2(this->x / right, this->y / right);
            }

            //Inline
			inline ScalarType Dot(const vec2 &rhs) const
			{
				return this->x * rhs.x + this->y * rhs.y;
			}

            inline ScalarType Length() const
            {
                return sqrt(this->x * this->x + this->y * this->y);
            }

            inline ScalarType LengthSquared() const
            {
                return this->x * this->x + this->y * this->y;
            }

            inline vec2 &Normalize()
            {
                *this = this->Normalized();
                return *this;
            }

            inline vec2 Normalized() const
            {
				ScalarType length = this->Length();
                return vec2(this->x / length, this->y / length);
            }

			inline vec2 Rotate(ScalarType angle) const;
        };

        //Binary operators
		template <typename ScalarType>
        inline Vector2<ScalarType> operator*(ScalarType left, const Vector2<ScalarType> &refRight)
        {
            return Vector2<ScalarType>(left * refRight.x, left * refRight.y);
        }

		typedef Vector2<float32> Vector2s;
    }
}