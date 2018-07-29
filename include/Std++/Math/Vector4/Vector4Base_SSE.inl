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
			return _mm_dp_ps(this->mmValue, right, 0xFF);
#else
#error "TODO: implement non-sse4.1 version"
#endif
		}
	};
}
#endif