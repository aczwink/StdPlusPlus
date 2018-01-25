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
#include "SSE.h"
#include <ACStdLib/Debug.h>
#include <ACStdLib/Definitions.h>
#include <ACStdLib/Memory.h>
#include <ACStdLib/Math/Vector2.hpp>

namespace ACStdLib
{
    class _MM_ALIGN16 vec2f64
    {
    public:
        //Members
        union
        {
            __m128d mmValue;
            float64 e[2];
            struct
            {
                float64 x;
                float64 y;
            };
        };

        //Constructors
        inline vec2f64()
        {
            this->mmValue = _mm_setzero_pd();
        }

        inline vec2f64(float64 x, float64 y)
        {
            this->mmValue = _mm_set_pd(y, x);
        }

        inline vec2f64(const Math::Vector2 &refV)
        {
            this->mmValue = _mm_set_pd(refV.y, refV.x);
        }

        inline vec2f64(__m128d mmValue)
        {
            this->mmValue = mmValue;
        }

        //Allocation
        /*
        static void* operator new(size_t size)
        {
            return MemAllocAligned(size, 16);
        }

        static void* operator new[](size_t size)
        {
            return MemAllocAligned(size, 16);
        }

        static void operator delete(void* ptr)
        {
            MemFreeAligned(ptr);
        }

        static void operator delete[](void* ptr)
        {
            MemFreeAligned(ptr);
        }
        */

        //Operators
        inline float64 &operator[](uint8 index)
        {
            ASSERT(index < 2, "Column must be < 2");

            return this->e[index];
        }

        inline float64 operator[](uint8 index) const
        {
            ASSERT(index < 2, "Column must be < 2");

            return this->e[index];
        }

        inline vec2f64 operator+(const vec2f64 &refOther) const
        {
            return _mm_add_pd(this->mmValue, refOther.mmValue);
        }

        inline vec2f64 operator-(const vec2f64 &refRight) const
        {
            return _mm_sub_pd(this->mmValue, refRight.mmValue);
        }

        inline vec2f64 operator*(const vec2f64 &refRight) const
        {
            return _mm_mul_pd(this->mmValue, refRight.mmValue);
        }

        inline vec2f64 operator/(float64 right) const
        {
            return _mm_div_pd(this->mmValue, _mm_set1_pd(right));
        }

        //Inline
        inline float64 LengthSquared() const
        {
            __m128d length;

            //dot
            length = _mm_mul_pd(this->mmValue, this->mmValue);
            length = _mm_hadd_pd(length, length);

            return _mm_cvtsd_f64(length);
        }
    };

    //Inline functions
    inline float64 Cross2D(const vec2f64 &refFirst, const vec2f64 &refSecond)
    {
        return refFirst.x * refSecond.y - refFirst.y * refSecond.x;
    }

    inline float64 Dot(const vec2f64 &refLeft, const vec2f64 &refRight)
    {
        __m128d result;

        result = _mm_mul_pd(refLeft.mmValue, refRight.mmValue);
        result = _mm_hadd_pd(result, result);

        return _mm_cvtsd_f64(result);
    }

    inline vec2f64 GetPerpendicularDirection(const vec2f64 &refV)
    {
        return vec2f64(refV.y, -refV.x);
    }

    inline float64 Length(const vec2f64 &refV)
    {
        __m128d length;

        //dot
        length = _mm_mul_pd(refV.mmValue, refV.mmValue);
        length = _mm_hadd_pd(length, length);
        //root
        length = _mm_sqrt_pd(length);

        return _mm_cvtsd_f64(length);
    }

    inline vec2f64 Normalize(const vec2f64 &refV)
    {
        __m128d norm;

        //dot
        norm = _mm_mul_pd(refV.mmValue, refV.mmValue);
        norm = _mm_hadd_pd(norm, norm);
        //root
        norm = _mm_sqrt_pd(norm);

        return _mm_div_pd(refV.mmValue, norm);
    }
}