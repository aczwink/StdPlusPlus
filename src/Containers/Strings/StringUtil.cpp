/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Corresponding header
#include <ACStdLib/Containers/Strings/StringUtil.h>
//Global
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
//Local
#include <ACStdLib/Containers/Strings/String.hpp>
#include <ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp>
#include <ACStdLib/Mathematics.h>
#include <ACStdLib/Math/Fraction.hpp>
//Namespaces
using namespace ACStdLib;

//Local Functions
static String ToString(uint64 value, int8 radix, uint8 nMinChars)
{
    uint32 rest;
    String buffer, result;

    if(value == 0)
        buffer = '0';

    while(value > 0)
    {
        rest = value % radix;
        value /= radix;

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

//Namespace Functions
String ACStdLib::FormatBitSize(uint32 bitSize, uint32 nFractionalDigits)
{
    String result;

    if(bitSize > MiB)
    {
        if(nFractionalDigits)
        {
            String str;
            uint32 length;

            str = ToString((float64)bitSize / MiB);

            length = MIN(str.GetLength(), str.FindReverse('.') + 1 + nFractionalDigits);
            return str.SubString(0, length) + " Mibit";
        }
        return String(bitSize / MiB) + " Mibit";
    }
    else if(bitSize > KiB)
    {
        if(nFractionalDigits)
        {
            String str;
            uint32 length;

            str = ToString((float64)bitSize / KiB);

            length = MIN(str.GetLength(), str.FindReverse('.') + 1 + nFractionalDigits);
            return str.SubString(0, length) + " Kibit";
        }
        return ToString((float64)bitSize / KiB) + " Kibit";
    }
    return ToString((uint64)bitSize) + " bit";
}

String ACStdLib::FormatFileSize(uint64 fileSize, uint32 nFractionalDigits)
{
    float64 fraction;
    String suffix;

    if(fileSize > MiB)
    {
        fraction = (float64)fileSize / MiB;
        fileSize /= MiB;
        suffix = " MiB";
    }
    else if(fileSize > KiB)
    {
        fraction = (float64)fileSize / KiB;
        fileSize /= KiB;
        suffix = " KiB";
    }
    else
    {
        nFractionalDigits = 0;
        suffix = " B";
    }

    if(nFractionalDigits)
    {
        uint32 length;
        String str;

        str = ToString(fraction);
        length = MIN(str.GetLength(), str.FindReverse('.') + 1 + nFractionalDigits);

        return str.SubString(0, length) + suffix;
    }

    return ToString(fileSize) + suffix;
}

bool ACStdLib::IsStringConvertibleToFloat(const ByteString &refString)
{
    char *pStr;

    strtod(refString.GetC_Str(), &pStr);

    return (refString.GetC_Str() + refString.GetLength()) == pStr;
}

bool ACStdLib::IsStringConvertibleToInteger(const ByteString &refString)
{
    uint32 i;

    i = 0;
    if(refString[0] == '+' || refString[0] == '-')
        i = 1;

    for(; i < refString.GetLength(); i++)
    {
        if(!IN_RANGE(refString[i], '0', '9'))
            return false;
    }

    return true;
}

int64 ACStdLib::StringToInt64(const String &refString)
{
    int64 result;

    result = 0;

    if(refString[0] == '-')
    {
        for(uint32 i = 1; i < refString.GetLength(); i++)
        {
            const char &refC = refString[i];
            if(!IN_RANGE(refC, '0', '9'))
                return -1;
            result -= Power((uint64)10, (uint64)(refString.GetLength() - i - 1)) * (refC - '0');
        }

        return result;
    }

    return StringToUInt64(refString);
}

uint64 ACStdLib::StringToUInt64(const ByteString &string)
{
	uint32 i;
	uint64 result;

	i = 0;
	result = 0;

	if(string[0] == '+')
		i++;

	for(; i < string.GetLength(); i++)
	{
		if(!IN_RANGE(string[i], '0', '9'))
			return -1;

		result += Power((uint64)10, (uint64)(string.GetLength() - i - 1)) * (string[i] - '0');
	}

	return result;
}

uint64 ACStdLib::StringToUInt64(const String &refString)
{
    uint32 i;
    uint64 result;

    i = 0;
    result = 0;

    if(refString[0] == '+')
        i++;

    for(; i < refString.GetLength(); i++)
    {
        if(!IN_RANGE(refString[i], '0', '9'))
            return -1;

        result += Power((uint64)10, (uint64)(refString.GetLength() - i - 1)) * (refString[i] - '0');
    }

    return result;
}

String ACStdLib::ToHexString(uint64 value, uint8 nMinChars)
{
    return "0x" + ::ToString(value, 16, nMinChars);
}

String ACStdLib::TimeToString(uint64 timeStamp, const Fraction &refTimeScale)
{
    uint64 usecs, hours, mins, secs;

    secs = timeStamp * refTimeScale.numerator / refTimeScale.denominator;
    usecs = (timeStamp * refTimeScale.numerator) % refTimeScale.denominator;
    mins = secs / 60;
    secs %= 60;
    hours = mins / 60;
    mins %= 60;

    return ToString(hours, 2) + ':' + ToString(mins, 2) + ':' + ToString(secs, 2) + '.' + ToString(100 * usecs * refTimeScale.numerator / refTimeScale.denominator, 2);
}

String ACStdLib::ToString(int64 value)
{
    ByteString buffer;
    bool isNegative = false;

    if(value == 0)
        return '0';

    if(value < 0)
    {
        isNegative = true;
        value = -value;
    }

    if(isNegative)
        return '-' + ToString((uint64)value);

    return ToString((uint64)value);
}

String ACStdLib::ToString(uint64 value, uint8 nMinChars)
{
    return ::ToString(value, 10, nMinChars);
}

String ACStdLib::ToString(float64 value)
{
    char buf[100];

    sprintf(buf, "%.*g", DBL_DIG, value);
    //sprintf_s(buf, sizeof(buf), "%.*g", DBL_DIG, value);

    return buf;
}

//8-bit functions
ByteString ACStdLib::To8BitString(const String &refString)
{
    ByteString tmp;

    tmp.Resize(refString.GetLength());
    for(uint32 i = 0; i < refString.GetLength(); i++)
    {
        if(refString[i] > UINT8_MAX)
            tmp[i] = '?'; //do what windows does
        else
            tmp[i] = (byte)refString[i];
    }

    return tmp;
}

ByteString ACStdLib::ToString_8Bit(uint64 value, uint8 nMinChars)
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