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
#ifdef XPC_FEATURE_SSE2
//Global
#include <emmintrin.h> //SSE2
#ifdef XPC_FEATURE_SSE3
#include <pmmintrin.h>
#endif
#ifdef XPC_FEATURE_SSE4_1
#include <smmintrin.h>
#endif

namespace _stdxx_
{
	template<>
	class Vector2Base<float64>
	{
	public:
		//Members
		union
		{
			struct
			{
				float64 x;
				float64 y;
			};
			float64 e[2];
			__m128d mmValue;
		};

	protected:
		//Inline
		inline __m128d _Dot_SSE(__m128d right) const
		{
#ifdef XPC_FEATURE_SSE4_1
			return _mm_dp_pd(this->mmValue, right, 0xFF);
#else
			__m128d result = _mm_mul_pd(this->mmValue, right);
#ifdef XPC_FEATURE_SSE3
			result = _mm_hadd_pd(result, result);
#else
#error "TODO: implement sse2 version"
#endif
			return result;
#endif
		}

		inline __m128d _Norm_SSE() const
		{
			return _mm_sqrt_pd(this->_NormSquared_SSE());
		}

		inline __m128d _NormSquared_SSE() const
		{
			return this->_Dot_SSE(this->mmValue);
		}
	};
}
#endif