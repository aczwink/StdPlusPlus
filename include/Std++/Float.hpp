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
#include "__Globaldependencies.h"
#include "Definitions.h"
#include "Mathematics.hpp"

namespace StdPlusPlus
{
    template<typename T>
    class Float
    {
    };

    template<>
    class Float<float32>
    {
    public:
        //Expressions
        static constexpr float32 Infinity()
        {
            return INFINITY;
        }
    };

    template<>
    class Float<float64>
	{
	public:
		//Expressions
		static constexpr bool AlmostEqual(float64 a, float64 b, float64 epsilon)
		{
			return Math::Abs(a - b) <= epsilon;
		}

#ifdef _STDPP_COMPILER_GCC
		static constexpr bool IsNotANumber(float64 f)
		{
			return __builtin_isnan(f);
		}
#endif

		/**
		 * The machine epsilon is the difference: 1.0 + 1 ULP - 1.0
		 * In other words, the machine epsilon is the difference of 1.0 and its next representable float value in the floating point system.
		 * More concretely, with base b and precision p, the machine epsilon yields = b^(1 - p)
		 * The machine epsilon is chosen like that because rounding at 1 gives the maximum relative rounding error.
		 *
		 * Note that this is the industry standard definition while other definitions define machine epsilon as b^(1 - p)/2.
		 * In this case machine epsilon is the largest value x such that 1.0 + x = 1.0 in the floating point system.
		 */
		static constexpr float64 MachineEpsilon()
		{
			return DBL_EPSILON;
		}

		/**
		 * Returns the quiet NaN value.
		 * Be careful, this can't be used for comparisons.
		 * @return
		 */
		static float64 NotANumber()
		{
			return nan("");
		}
	};
}