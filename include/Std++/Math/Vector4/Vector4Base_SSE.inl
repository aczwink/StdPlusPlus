/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#ifdef XPC_FEATURE_SSE2
//Global
#include <emmintrin.h> //SSE2
#ifdef XPC_FEATURE_SSE4_1
#include <smmintrin.h>
#endif

namespace _stdxx_
{
	template<>
	class Vector4Base<float32>
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

	protected:
		//Inline
		inline __m128 _Dot_SSE(__m128 right) const
		{
#ifdef XPC_FEATURE_SSE4_1
			return _mm_dp_ps(this->mmValue, right, 0xF1);
#else
#error "TODO: implement non-sse4.1 version"
			/*
			 * //dot
            norm = _mm_mul_ps(this->mmValue, this->mmValue);
            norm = _mm_hadd_ps(norm, norm);
            norm = _mm_hadd_ps(norm, norm);
			 */
#endif
		}

		inline __m128 _Norm_SSE() const
		{
			return _mm_sqrt_ss(this->_NormSquared_SSE());
		}

		inline __m128 _NormSquared_SSE() const
		{
			return this->_Dot_SSE(this->mmValue);
		}
	};
}
#endif

/*
#pragma once
//Local
#include <Std++/Debug.hpp>
#include <Std++/Definitions.h>
#include <Std++/Math/Vector3.hpp>
#include <Std++/Math/Vector4.hpp>

namespace StdXX
{
    class alignas(16) vec4f32
    {
    public:
        //Constructors
        inline vec4f32()
        {
            this->mmValue = _mm_setzero_ps();
        }

        inline vec4f32(__m128 mmValue)
        {
            this->mmValue = mmValue;
        }

        inline vec4f32(const Math::Vector3<float32> &refVec3, float32 w)
        {
            this->mmValue = _mm_set_ps(w, refVec3.z, refVec3.y, refVec3.x);
        }

        inline vec4f32(const Math::Vector4<float32> &refVec)
        {
            this->mmValue = _mm_set_ps(refVec.w, refVec.z, refVec.y, refVec.x);
        }

        inline vec4f32(float32 x, float32 y, float32 z, float32 w)
        {
            this->mmValue = _mm_set_ps(w, z, y, x);
        }

        //Operators
        inline vec4f32 operator+(const vec4f32 &refOther) const
        {
            return _mm_add_ps(this->mmValue, refOther.mmValue);
        }

        inline vec4f32 operator-(const vec4f32 &refOther) const
        {
            return _mm_sub_ps(this->mmValue, refOther.mmValue);
        }

        inline vec4f32 operator*(const vec4f32 &refRight) const
        {
            return _mm_mul_ps(this->mmValue, refRight.mmValue);
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
    };
}*/