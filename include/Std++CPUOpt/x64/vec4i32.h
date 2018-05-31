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
#include <Std++/Debug.hpp>
#include <Std++/Definitions.h>

namespace StdPlusPlus
{
    class _MM_ALIGN16 vec4i32
    {
    public:
        //Members
        union
        {
            __m128i mmValue;
            int32 e[4];
            struct
            {
                int32 a;
                int32 b;
                int32 c;
                int32 d;
            };
        };

        //Constructors
        inline vec4i32()
        {
            this->mmValue = _mm_setzero_si128();
        }

        inline vec4i32(int32 a, int32 b, int32 c, int32 d)
        {
            this->mmValue = _mm_set_epi32(d, c, b, a);
        }

        inline vec4i32(__m128i mmValue)
        {
            this->mmValue = mmValue;
        }

        //Inline operators
        inline vec4i32 operator+(const vec4i32 &refV) const
        {
            return _mm_add_epi32(this->mmValue, refV.mmValue);
        }

        inline vec4i32 operator-(const vec4i32 &refV) const
        {
            return _mm_sub_epi32(this->mmValue, refV.mmValue);
        }

        inline vec4i32 operator*(int32 v) const
        {
            return _mm_mullo_epi32(this->mmValue, _mm_set1_epi32(v));
        }

        inline vec4i32 operator*(const vec4i32 &refV) const
        {
            return _mm_mullo_epi32(this->mmValue, refV.mmValue);
        }

        inline vec4i32 operator/(int32 v) const
        {
            return vec4i32(this->a / v, this->b / v, this->c / v, this->d / v);
        }

        //Inline methods
        inline int32 Sum() const
        {
            __m128i tmp;

            tmp = _mm_hadd_epi32(this->mmValue, this->mmValue);

            return _mm_extract_epi32(_mm_hadd_epi32(tmp, tmp), 0); //SSE2: _mm_cvtsi128_si32
        }
    };

    //Class right hand side operators
    inline vec4i32 operator*(int32 v, const vec4i32 &refV)
    {
        return _mm_mullo_epi32(refV.mmValue, _mm_set1_epi32(v));
    }
}