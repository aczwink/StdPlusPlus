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
#include "../Definitions.h"
#include "../Debug.h"

namespace StdPlusPlus
{
    namespace Math
    {
        //Forward declarations
        class Vector3;

        class STDPLUSPLUS_API Vector4
        {
        public:
            //Members
            union
            {
                struct
                {
                    float32 x;
                    float32 y;
                    float32 z;
                    float32 w;
                };
                float32 e[4];
            };

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

            Vector4(const Vector3 &refXYZ, float32 w);

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

            inline Vector4 operator-(const Vector4 &refRight) const
            {
                return Vector4(this->x - refRight.x, this->y - refRight.y, this->z - refRight.z, this->w - refRight.w);
            }

            inline Vector4 operator*(const Vector4 &refRight) const
            {
                return Vector4(this->x * refRight.x, this->y * refRight.y, this->z * refRight.z, this->w * refRight.w);
            }

            inline Vector4 &operator*=(float32 right)
            {
                this->x *= right;
                this->y *= right;
                this->z *= right;
                this->w *= right;

                return *this;
            }
        };
    }
}