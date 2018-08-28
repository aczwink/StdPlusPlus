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
#include "ByteString.hpp"

namespace StdXX
{
    //Move declarations
    class Fraction;
    class OldString;

    OldString ToString(uint64 i, uint8 bla = 0);

    //Functions
    bool IsStringConvertibleToFloat(const ByteString &refString);
    bool IsStringConvertibleToInteger(const ByteString &refString);
    STDPLUSPLUS_API int64 StringToInt64(const OldString &refString);
    STDPLUSPLUS_API uint64 StringToUInt64(const OldString &refString);
    STDPLUSPLUS_API OldString ToHexString(uint64 value, uint8 nMinChars = 0, bool addBase = true);

    //8-bit functions
    STDPLUSPLUS_API ByteString To8BitString(const OldString &refString);
    STDPLUSPLUS_API ByteString ToString_8Bit(uint64 value, uint8 nMinChars = 0);

    //Inline Functions
    template <typename CharType>
    inline uint32 GetStringLength(const CharType *pStr)
    {
        const CharType *pEnd;

        for(pEnd = pStr; *pEnd; pEnd++);

        return uint32(pEnd - pStr);
    }
}