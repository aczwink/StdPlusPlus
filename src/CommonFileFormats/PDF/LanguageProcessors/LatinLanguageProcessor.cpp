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
//Class header
#include "LatinLanguageProcessor.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

bool LatinLanguageProcessor::IsInLanguage(uint16 c) const
{
	//http://en.wikipedia.org/wiki/Unicode_block

	//private use area
	if(Math::IsValueInInterval(c, 0xE000_u16, 0xF8FF_u16))
		return true;

	return c <= 0x17F;
}

void LatinLanguageProcessor::Process(uint16 *pInput, uint16 nInputChars, DynamicArray<uint16> &refOutChars) const
{
	while(nInputChars--)
	{
		refOutChars.Push(*pInput++);
	}
}