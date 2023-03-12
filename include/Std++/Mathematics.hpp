/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Math/Radian.hpp>
#include "Debug.hpp"
#include "Definitions.h"
#include "__Globaldependencies.h"

//Constants
#define PI 3.1415926535897932384626433832795

namespace StdXX
{
	namespace Math
	{
		template<typename T>
		constexpr T Abs(const T &x)
		{
			return ( (x) < 0 ) ? -(x) : (x);
		}

		template<typename T>
		constexpr bool IsValueInInterval(const T &x, const T &min, const T &max)
		{
			return ((x) >= (min) && (x) <= (max));
		}

		template<typename T>
		constexpr Radian<T> ArcCos(const T &x)
		{
			ASSERT(IsValueInInterval(x, T(-1), T(1)), u8"ArcCos input must be in range [-1, 1]");
			return acos(x);
		}

		template<typename T>
		constexpr T Clamp(const T &x, const T &min, const T &max)
		{
			return (((x) < (min)) ? (min) : (((x) > (max)) ? (max) : (x)));
		}

		template<typename T>
		constexpr T Cot(const T &x)
		{
			return 1 / tan(x);
		}

		template<typename T>
		constexpr T Log(const T& base, const T& value)
		{
			T temp = value;
			T result = 0;
			T zero = 0;
			while(temp != zero)
			{
				temp /= base;
				++result;
			}

			return result;
		}

		template<typename T>
		constexpr T Max(const T &a, const T &b)
		{
			return ((a) > (b)) ? (a) : (b);
		}

		template<typename T>
		constexpr T Min(const T &a, const T &b)
		{
			return ((a) < (b)) ? (a) : (b);
		}

		template<typename T>
		constexpr T Power(const T& base, const T& exponent)
		{
			ASSERT(exponent >= 0, u8"This method does only work for integers. This infers exponent > 0.");
			T result = 1;
			for (T i = 0; i < exponent; ++i)
				result *= base;
			return result;
		}

		template <typename T>
		constexpr T Sign(T x)
		{
			return (x == 0) ? 0 : ( (x < 0) ? -1 : 1 );
		}
	}
}