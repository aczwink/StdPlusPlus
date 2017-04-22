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
#pragma once
//Local
#include "C8BitString.h"

namespace ACStdLib
{
    //Forward declarations
    class CFraction;
    class CString;

    //Functions
    ACSTDLIB_API CString FormatBitSize(uint32 bitSize, uint32 nFractionalDigits = 0);
    ACSTDLIB_API CString FormatFileSize(uint64 fileSize, uint32 nFractionalDigits = 2);
    bool IsStringConvertibleToFloat(const C8BitString &refString);
    bool IsStringConvertibleToInteger(const C8BitString &refString);
    ACSTDLIB_API float64 StringToFloat64(const CString &refString);
    ACSTDLIB_API int64 StringToInt64(const CString &refString);
    ACSTDLIB_API uint64 StringToUInt64(const CString &refString);
    ACSTDLIB_API CString TimeToString(uint64 timeStamp, const CFraction &refTimeScale);
    ACSTDLIB_API CString ToHexString(uint64 value, uint8 nMinChars = 0);
    CString ToString(int64 value);
    ACSTDLIB_API CString ToString(uint64 value, uint8 nMinChars = 0);
    ACSTDLIB_API CString ToString(float64 value);

    //8-bit functions
    ACSTDLIB_API C8BitString To8BitString(const CString &refString);
    ACSTDLIB_API C8BitString ToString_8Bit(uint64 value, uint8 nMinChars = 0);

    //Inline Functions
    template <typename CharType>
    inline uint32 GetStringLength(const CharType *pStr)
    {
        const CharType *pEnd;

        for(pEnd = pStr; *pEnd; pEnd++);

        return uint32(pEnd - pStr);
    }

    inline uint32 ToLowercase(uint32 codePoint)
    {
        if(IN_RANGE(codePoint, 'A', 'Z'))
            return codePoint + 32;
        return codePoint;
    }
}