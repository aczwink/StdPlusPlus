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
#include "../Debug.h"

namespace StdPlusPlus
{
    //Move declarations
    class vec2f64;

    namespace Math
    {
        class Vector2
        {
        public:
            //Members
            union
            {
                struct
                {
                    float32 x;
                    float32 y;
                };
                float32 e[2];
            };

            //Constructors
            inline Vector2()
            {
                this->x = 0;
                this->y = 0;
            }

            inline Vector2(float32 x, float32 y)
            {
                this->x = x;
                this->y = y;
            }

            Vector2(const vec2f64 &refV);

            //Operators
            inline float32 &operator[](uint8 idx)
            {
                ASSERT(idx < 2, "Column must be < 2");

                return this->e[idx];
            }

            inline const float32 &operator[](uint8 idx) const
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

            inline Vector2 operator*(float32 right) const
            {
                return Vector2(this->x * right, this->y * right);
            }

            inline Vector2 operator*(const Vector2 &refRight) const
            {
                return Vector2(this->x * refRight.x, this->y * refRight.y);
            }

            inline Vector2 &operator*=(float32 right)
            {
                this->x *= right;
                this->y *= right;

                return *this;
            }

            inline Vector2 operator/(float32 right) const
            {
                return Vector2(this->x / right, this->y / right);
            }

            //Inline
            inline float32 Length() const
            {
                return sqrtf(this->x * this->x + this->y * this->y);
            }

            inline float32 LengthSquared() const
            {
                return this->x * this->x + this->y * this->y;
            }

            inline Vector2 Normalize() const
            {
                float32 length;

                length = this->Length();

                return Vector2(this->x / length, this->y / length);
            }
        };

        //Binary operators
        inline Vector2 operator*(float32 left, const Vector2 &refRight)
        {
            return Vector2(left * refRight.x, left * refRight.y);
        }
    }
}