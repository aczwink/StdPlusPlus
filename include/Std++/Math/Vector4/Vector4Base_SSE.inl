/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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