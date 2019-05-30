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
#include "Vector2.hpp"

namespace StdXX
{
	namespace Math
	{
		//Constructors
		template<>
		inline Vector2<float64>::Vector2()
		{
			this->mmValue = _mm_setzero_pd();
		}

		template<>
		inline Vector2<float64>::Vector2(float64 x, float64 y)
		{
			this->mmValue = _mm_set_pd(y, x);
		}

		//Arithmetic operators
		template<>
		inline Vector2<float64> Vector2<float64>::operator+(const Vector2<float64> &refOther) const
		{
			return _mm_add_pd(this->mmValue, refOther.mmValue);
		}

		template <>
		inline Vector2<float64> Vector2<float64>::operator-(const Vector2<float64> &refRight) const
		{
			return _mm_sub_pd(this->mmValue, refRight.mmValue);
		}

		template <>
		inline Vector2<float64> Vector2<float64>::operator*(const Vector2<float64> &refRight) const
		{
			return _mm_mul_pd(this->mmValue, refRight.mmValue);
		}

		template <>
		inline Vector2<float64> Vector2<float64>::operator*(float64 right) const
		{
			return _mm_mul_pd(this->mmValue, _mm_set1_pd(right));
		}

		template <>
		inline Vector2<float64> &Vector2<float64>::operator*=(float64 right)
		{
			this->mmValue = _mm_mul_pd(this->mmValue, _mm_set1_pd(right));

			return *this;
		}

		template <>
		inline Vector2<float64> Vector2<float64>::operator/(float64 right) const
		{
			return _mm_div_pd(this->mmValue, _mm_set1_pd(right));
		}

		//Comparison operators
		template <>
		inline bool Vector2<float64>::operator==(const Vector2<float64> &other) const
		{
			return _mm_movemask_pd(_mm_cmpeq_pd(this->mmValue, other.mmValue)) == 3;
		}

		template <>
		inline bool Vector2<float64>::operator!=(const Vector2<float64> &other) const
		{
			return _mm_movemask_pd(_mm_cmpneq_pd(this->mmValue, other.mmValue)) == 3;
		}

		//Inline
		template <>
		inline float64 Vector2<float64>::Dot(const Vector2<float64> &refRight) const
		{
			return _mm_cvtsd_f64(this->_Dot_SSE(refRight.mmValue));
		}

		template <>
		inline float64 Vector2<float64>::Length() const
		{
			return _mm_cvtsd_f64(this->_Norm_SSE());
		}

		template <>
		inline float64 Vector2<float64>::LengthSquared() const
		{
			return _mm_cvtsd_f64(this->_NormSquared_SSE());
		}

		template <>
		inline Vector2<float64> Vector2<float64>::Normalized() const
		{
			return _mm_div_pd(this->mmValue, this->_Norm_SSE());
		}
	}
}
#endif