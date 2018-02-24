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
#include "SSE.h"
#include <Std++/Debug.h>
#include <Std++/Definitions.h>
#include <Std++/Math/Vector3.hpp>
#include <Std++/Math/Vector4.hpp>

namespace StdPlusPlus
{
    class _MM_ALIGN16 vec4f32
    {
    public:
        //Members
        union
        {
            __m128 mmValue;
            float32 e[4];
            struct
            {
                float32 x;
                float32 y;
                float32 z;
                float32 w;
            };
        };

        //Constructors
        inline vec4f32()
        {
            this->mmValue = _mm_setzero_ps();
        }

        inline vec4f32(__m128 mmValue)
        {
            this->mmValue = mmValue;
        }

        inline vec4f32(const Math::Vector3 &refVec3, float32 w)
        {
            this->mmValue = _mm_set_ps(w, refVec3.z, refVec3.y, refVec3.x);
        }

        inline vec4f32(const Math::Vector4 &refVec)
        {
            this->mmValue = _mm_set_ps(refVec.w, refVec.z, refVec.y, refVec.x);
        }

        inline vec4f32(float32 x, float32 y, float32 z, float32 w)
        {
            this->mmValue = _mm_set_ps(w, z, y, x);
        }

        //Operators
        inline float32 &operator[](uint8 index)
        {
            ASSERT(index < 4, "Column must be < 4");

            return this->e[index];
        }

        inline float32 operator[](uint8 index) const
        {
            ASSERT(index < 4, "Column must be < 4");

            return this->e[index];
        }

        inline vec4f32 operator+(const vec4f32 &refOther) const
        {
            return _mm_add_ps(this->mmValue, refOther.mmValue);
        }

        inline vec4f32 &operator+=(const vec4f32 &refOther)
        {
            this->mmValue = _mm_add_ps(this->mmValue, refOther.mmValue);

            return *this;
        }

        inline vec4f32 operator-(const vec4f32 &refOther) const
        {
            return _mm_sub_ps(this->mmValue, refOther.mmValue);
        }

        inline vec4f32 operator*(float32 right) const
        {
            return _mm_mul_ps(this->mmValue, _mm_set_ps1(right));
        }

        inline vec4f32 operator*(const vec4f32 &refRight) const
        {
            return _mm_mul_ps(this->mmValue, refRight.mmValue);
        }

        inline vec4f32 operator/(float32 right)
        {
            return _mm_div_ps(this->mmValue, _mm_set_ps1(right));
        }

        inline vec4f32 operator/(const vec4f32 &refRight)
        {
            return _mm_div_ps(this->mmValue, refRight.mmValue);
        }

        inline vec4f32 &operator/=(float32 right)
        {
            this->mmValue = _mm_div_ps(this->mmValue, _mm_set_ps1(right));

            return *this;
        }

        //Methods
        inline float32 Length() const
        {
            return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(this->mmValue, this->mmValue, 0xF1)));
        }

        inline vec4f32 Normalize() const
        {
            __m128 norm;

            //dot
            norm = _mm_mul_ps(this->mmValue, this->mmValue);
            norm = _mm_hadd_ps(norm, norm);
            norm = _mm_hadd_ps(norm, norm);
            //root
            norm = _mm_sqrt_ps(norm);

            return _mm_div_ps(this->mmValue, norm);
        }
    };

    //LHS-Operators
    inline vec4f32 operator*(float32 left, const vec4f32 &refRight)
    {
        return refRight * left;
    }

    //Functions
    inline vec4f32 Cross(const vec4f32 &refA, const vec4f32 &refB)
    {
        return _mm_sub_ps(
                _mm_mul_ps(
                        _mm_shuffle_ps(refA.mmValue, refA.mmValue, _MM_SHUFFLE(3, 0, 2, 1)),
                        _mm_shuffle_ps(refB.mmValue, refB.mmValue, _MM_SHUFFLE(3, 1, 0, 2))
                ),
                _mm_mul_ps(
                        _mm_shuffle_ps(refA.mmValue, refA.mmValue, _MM_SHUFFLE(3, 1, 0, 2)),
                        _mm_shuffle_ps(refB.mmValue, refB.mmValue, _MM_SHUFFLE(3, 0, 2, 1))
                )
        );
    }

    inline float32 Dot(const vec4f32 &refA, const vec4f32 &refB)
    {
        return _mm_cvtss_f32(_mm_dp_ps(refA.mmValue, refB.mmValue, 0xF1));
    }
}