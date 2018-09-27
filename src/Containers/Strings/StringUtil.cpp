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
//Corresponding header
#include <Std++/Containers/Strings/StringUtil.h>
//Global
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
//Local
#include <Std++/Containers/Strings/OldString.hpp>
#include <Std++/Containers/Strings/UTF-8/UTF8String.hpp>
#include <Std++/Mathematics.hpp>
#include <Std++/Math/Fraction.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;

OldString StdXX::ToString(uint64 i, uint8 bla)
{
	NOT_IMPLEMENTED_ERROR; //TODO: alter and change into new string class
	return OldString();
}

//Namespace Functions
bool StdXX::IsStringConvertibleToFloat(const ByteString &refString)
{
    char *pStr;

    strtod(refString.GetC_Str(), &pStr);

    return (refString.GetC_Str() + refString.GetLength()) == pStr;
}

bool StdXX::IsStringConvertibleToInteger(const ByteString &refString)
{
    uint32 i;

    i = 0;
    if(refString[0] == '+' || refString[0] == '-')
        i = 1;

    for(; i < refString.GetLength(); i++)
    {
        if(!IsValueInInterval(refString[i], '0', '9'))
            return false;
    }

    return true;
}

int64 StdXX::StringToInt64(const OldString &refString)
{
	NOT_IMPLEMENTED_ERROR; //deprecated
	return 0;
}

uint64 StdXX::StringToUInt64(const OldString &refString)
{
	NOT_IMPLEMENTED_ERROR; //deprecated
	return 0;
}

OldString StdXX::ToHexString(uint64 value, uint8 nMinChars, bool addBase)
{
	NOT_IMPLEMENTED_ERROR; //TODO: goes to new string class
	/*
	if(addBase)
		return "0x" + ::ToString(value, 16, nMinChars);
	return ::ToString(value, 16, nMinChars);
	 */
	return OldString();
}

//8-bit functions
ByteString StdXX::To8BitString(const OldString &refString)
{
    ByteString tmp;

    tmp.Resize(refString.GetLength());
    for(uint32 i = 0; i < refString.GetLength(); i++)
    {
        if(refString[i] > Natural<uint8>::Max())
            tmp[i] = '?'; //do what windows does
        else
            tmp[i] = (byte)refString[i];
    }

    return tmp;
}

ByteString StdXX::ToString_8Bit(uint64 value, uint8 nMinChars)
{
    uint32 rest;
    ByteString buffer, result;

    if(value == 0)
        buffer = '0';

    while(value > 0)
    {
        rest = value % 10;
        value /= 10;

        if(rest > 9) //for hex
            buffer += (char)(rest - 10 + 'A');
        else
            buffer += (char)(rest + '0');
    }

    if(buffer.GetLength() < nMinChars)
    {
        uint8 nCharsToFill;

        nCharsToFill = nMinChars - buffer.GetLength();

        for(uint32 i = 0; i < nCharsToFill; i++)
            buffer += '0';
    }

    result.EnsureCapacity(buffer.GetLength());

    for(uint32 i = 0; i < buffer.GetLength(); i++)
        result += (char)buffer[buffer.GetLength() - i - 1];

    return result;
}