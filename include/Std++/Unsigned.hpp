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
#include "Debug.hpp"
#include "Definitions.h"

namespace StdXX
{
	template<typename T>
	class Unsigned
	{
	};

	template<>
	class Unsigned<uint8>
	{
	public:
		//Expressions
		static constexpr uint8 Max()
		{
			return ((uint8)0xFF);
		}
	};

	template<>
	class Unsigned<uint32>
	{
	public:
		//Expressions
		static constexpr uint32 DivCeil(uint32 dividend, uint32 divisor)
		{
			return (dividend + (divisor - 1)) / divisor; //TODO: this might actually overflow, whereas dividend / divisor does not
		}

		static constexpr uint32 From4UInt8(uint8 msb, uint8 upperMiddle, uint8 lowerMiddle, uint8 lsb)
		{
			return (((msb) << 24) | ((upperMiddle) << 16) | ((lowerMiddle) << 8) | (lsb));
		}

		static constexpr uint32 Max()
		{
			return ((uint32)0xFFFFFFFF);
		}
	};

	template<>
	class Unsigned<uint64>
	{
	public:
		//Expressions
		static constexpr uint64 Concat(uint32 high32, uint32 low32)
		{
			return ( uint64(low32) | (uint64(high32) << 32u) );
		}

		static constexpr uint32 Low32(uint64 v)
		{
			return uint32(v & Unsigned<uint32>::Max());
		}

		static constexpr uint32 High32(uint64 v)
		{
			return uint32(v >> 32u);
		}

		static constexpr uint64 Max()
		{
			return ((uint64)0xFFFFFFFFFFFFFFFF);
		}
	};

	//Literals
	inline uint8 operator "" u8(unsigned long long value)
	{
		ASSERT(value <= Unsigned<uint8>::Max(), u8"Too large uint8 literal");
		return static_cast<uint8>(value);
	}
}