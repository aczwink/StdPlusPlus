/*
 * Copyright (c) 2017-2018,2021 Amir Czwink (amir130@hotmail.de)
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
	inline bool IsDecimalDigitChar(uint32 codePoint)
	{
		return (codePoint >= u8'0') and (codePoint <= u8'9');
	}

	inline bool IsWhiteSpaceChar(uint32 codePoint)
	{
		switch(codePoint)
		{
			case u8' ':
			case u8'\t':
				return true;
		}

		return Math::IsValueInInterval(codePoint, (uint32)0xA, (uint32)0xD); //\n, \v, \f, \r
	}

	inline uint32 ToLowercase(uint32 codePoint)
	{
		if(Math::IsValueInInterval(codePoint, (uint32)u8'A', (uint32)u8'Z'))
			return codePoint + 32;
		return codePoint;
	}

	inline uint32 ToUppercase(uint32 codePoint)
	{
		if(Math::IsValueInInterval(codePoint, (uint32)u8'a', (uint32)u8'z'))
			return codePoint - 32;
		return codePoint;
	}
}