/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include "../../__Globaldependencies.h" //needed for sqrt
#include "../../Definitions.h"
#include "../../Debug.h"

namespace ACStdLib
{
    //Forward declarations
    class vec2f64;

    namespace Math
    {
        class CVector2
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
            inline CVector2()
            {
                this->x = 0;
                this->y = 0;
            }

            inline CVector2(float32 x, float32 y)
            {
                this->x = x;
                this->y = y;
            }

            CVector2(const vec2f64 &refV);

            //Operators
            inline float32 &operator[](uint8 idx)
            {
                ASSERT(idx < 2);

                return this->e[idx];
            }

            inline const float32 &operator[](uint8 idx) const
            {
                ASSERT(idx < 2);

                return this->e[idx];
            }

            inline CVector2 operator-()
            {
                return CVector2(-this->x, -this->y);
            }

            //Binary operators
            inline CVector2 operator+(const CVector2 &refRight) const
            {
                return CVector2(this->x + refRight.x, this->y + refRight.y);
            }

            inline CVector2 operator-(const CVector2 &refRight) const
            {
                return CVector2(this->x - refRight.x, this->y - refRight.y);
            }

            inline CVector2 operator*(float32 right) const
            {
                return CVector2(this->x * right, this->y * right);
            }

            inline CVector2 operator*(const CVector2 &refRight) const
            {
                return CVector2(this->x * refRight.x, this->y * refRight.y);
            }

            inline CVector2 &operator*=(float32 right)
            {
                this->x *= right;
                this->y *= right;

                return *this;
            }

            inline CVector2 operator/(float32 right) const
            {
                return CVector2(this->x / right, this->y / right);
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

            inline CVector2 Normalize() const
            {
                float32 length;

                length = this->Length();

                return CVector2(this->x / length, this->y / length);
            }
        };

        //Binary operators
        inline CVector2 operator*(float32 left, const CVector2 &refRight)
        {
            return CVector2(left * refRight.x, left * refRight.y);
        }
    }
}