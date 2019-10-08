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
#pragma once
//Local
#include <Std++/Mathematics.hpp>
#include "Definitions.h"

namespace StdXX
{
    template<typename T>
    class Signed
    {
    };

	template<>
	class Signed<int8>
	{
	public:
		//Expressions
		static constexpr int8 Max()
		{
			return ((int8)127);
		}

		static constexpr int8 Min()
		{
			return ((int8)-128);
		}
	};

    template<>
    class Signed<int16>
    {
    public:
        //Expressions
        static constexpr int16 Max()
        {
            return ((int16)32767);
        }

        static constexpr int16 Min()
        {
            return ((int16)-32768);
        }
    };

    template<>
    class Signed<int32>
    {
    public:
        //Expressions
        static constexpr int32 Max()
        {
            return ((int32)0x7FFFFFFF);
        }

        static constexpr int32 Min()
        {
            return ((int32)-2147483648);
        }
    };

	template<>
	class Signed<int64>
	{
	public:
		//Expressions
		static constexpr int64 Max()
		{
			return int64(0x7FFFFFFFFFFFFFFF);
		}

		static constexpr int64 Min()
		{
			return int64(0x8000000000000000);
		}
	};

	//Literals
	inline int8 operator "" _i8(unsigned long long value)
	{
		ASSERT(Math::IsValueInInterval((long long)value, (long long)Signed<int8>::Min(), (long long)Signed<int8>::Max()), u8"Value is out of range for int8");
		return static_cast<int8>(value);
	}
}