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
//Local
#include "Vector4.hpp"

namespace StdXX
{
	namespace Math
	{
		//Operators
		template<>
		inline Vector4<float32> Vector4<float32>::operator*(float32 scalar) const
		{
			return _mm_mul_ps(this->mmValue, _mm_set_ps1(scalar));
		}

		template<>
		inline Vector4<float32> Vector4<float32>::operator/(float32 scalar) const
		{
			return _mm_div_ps(this->mmValue, _mm_set_ps1(scalar));
		}

		//Inline
		template<>
		inline Vector4<float32> Vector4<float32>::Cross(const Vector4<float32> &rhs) const
		{
			return _mm_sub_ps(
					_mm_mul_ps(
							_mm_shuffle_ps(this->mmValue, this->mmValue, _MM_SHUFFLE(3, 0, 2, 1)),
							_mm_shuffle_ps(rhs.mmValue, rhs.mmValue, _MM_SHUFFLE(3, 1, 0, 2))
					),
					_mm_mul_ps(
							_mm_shuffle_ps(this->mmValue, this->mmValue, _MM_SHUFFLE(3, 1, 0, 2)),
							_mm_shuffle_ps(rhs.mmValue, rhs.mmValue, _MM_SHUFFLE(3, 0, 2, 1))
					)
			);
		}

		template<>
		inline float32 Vector4<float32>::Dot(const Vector4<float32> &rhs) const
		{
			return _mm_cvtss_f32(this->_Dot_SSE(rhs.mmValue));
		}

		template <>
		inline float32 Vector4<float32>::Length() const
		{
			return _mm_cvtss_f32(this->_Norm_SSE());
		}

		template <>
		inline Vector4<float32> Vector4<float32>::Normalized() const
		{
			return _mm_div_pd(this->mmValue, this->_Norm_SSE());
		}
	}
}
#endif