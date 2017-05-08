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
    namespace Math
    {
        //Forward declarations
        class CVector4;

        class ACSTDLIB_API Vector3
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
                };
                float32 e[3];
            };

            //Constructors
            inline Vector3()
            {
                this->x = 0;
                this->y = 0;
                this->z = 0;
            }

            inline Vector3(float32 x, float32 y, float32 z)
            {
                this->x = x;
                this->y = y;
                this->z = z;
            }

            Vector3(const CVector4 &refV);

            //Operators
            inline float32 &operator[](uint8 idx)
            {
                ASSERT(idx < 3);

                return this->e[idx];
            }

            inline const float32 &operator[](uint8 idx) const
            {
                ASSERT(idx < 3);

                return this->e[idx];
            }

            inline Vector3 &operator=(const Vector3 &refOther)
            {
                this->x = refOther.x;
                this->y = refOther.y;
                this->z = refOther.z;

                return *this;
            }

            inline Vector3 &operator+=(const Vector3 &refOther)
            {
                this->x += refOther.x;
                this->y += refOther.y;
                this->z += refOther.z;

                return *this;
            }

            inline Vector3 &operator-=(const Vector3 &refOther)
            {
                this->x -= refOther.x;
                this->y -= refOther.y;
                this->z -= refOther.z;

                return *this;
            }

            inline Vector3 operator-() const
            {
                return Vector3(-this->x, -this->y, -this->z);
            }

            //Methods
            Vector3 Cross(const Vector3 &refRight) const;

            //Inline
            inline float32 Length() const
            {
                return sqrtf(this->SquaredLength());
            }

            inline Vector3 Normalize() const
            {
                float32 length;

                length = this->Length();

                return Vector3(this->x / length, this->y / length, this->z / length);
            }

            inline float32 SquaredLength() const
            {
                return this->x * this->x + this->y * this->y + this->z * this->z;
            }
        };

        //Class operators
        inline Vector3 operator+(const Vector3 &refLeft, const Vector3 &refRight)
        {
            return Vector3(refLeft.x + refRight.x, refLeft.y + refRight.y, refLeft.z + refRight.z);
        }

        inline Vector3 operator-(const Vector3 &refLeft, const Vector3 &refRight)
        {
            return Vector3(refLeft.x - refRight.x, refLeft.y - refRight.y, refLeft.z - refRight.z);
        }

        inline Vector3 operator*(float32 scalar, const Vector3 &refRight)
        {
            return Vector3(scalar * refRight.x, scalar * refRight.y, scalar * refRight.z);
        }

        inline Vector3 operator*(const Vector3 &refLeft, float32 scalar)
        {
            return Vector3(scalar * refLeft.x, scalar * refLeft.y, scalar * refLeft.z);
        }

        //Inline Functions
        inline Vector3 Cross(const Vector3 &refLeft, const Vector3 &refRight)
        {
            return Vector3(refLeft.y * refRight.z - refLeft.z * refRight.y, refLeft.z * refRight.x - refLeft.x * refRight.z, refLeft.x * refRight.y - refLeft.y * refRight.x);
        }

        inline float32 Dot(const Vector3 &refLeft, const Vector3 &refRight)
        {
            return float32((float64)refLeft.x * (float64)refRight.x + (float64)refLeft.y * (float64)refRight.y + (float64)refLeft.z * (float64)refRight.z);
        }
    }
}