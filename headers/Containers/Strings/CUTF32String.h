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
#include "AFixedCharLengthString.h"

namespace ACStdLib
{
    //Forward declarations
    class C8BitString;
    class CUTF8String;

    class ACSTDLIB_API CUTF32String : public AFixedCharLengthString<uint32>
    {
    public:
        //Constructors
        inline CUTF32String()
        {
        }

        inline CUTF32String(const CUTF32String &refString) //copy ctor
        {
            *this = refString;
        }

        inline CUTF32String(CUTF32String &&refString) //move ctor
        {
            *this = refString;
        }

        inline CUTF32String(char c)
        {
            *this = (uint8)c;
        }

        inline CUTF32String(const char *pString)
        {
            *this = (const uint8 *)pString;
        }

        inline CUTF32String(const uint8 *pString)
        {
            *this = pString;
        }

        inline CUTF32String(const uint16 *pStr)
        {
            *this = pStr;
        }

        inline CUTF32String(const C8BitString &refStr)
        {
            *this = refStr;
        }

        inline CUTF32String(const CUTF8String &refString)
        {
            *this = refString;
        }

        //Operators
        CUTF32String &operator=(const CUTF32String &refString); //copy assign
        CUTF32String &operator=(CUTF32String &&refString); //move assign
        CUTF32String &operator=(uint8 c);
        CUTF32String &operator=(const uint8 *pString);
        CUTF32String &operator=(const uint16 *pStr);
        CUTF32String &operator=(const C8BitString &refStr);
        CUTF32String &operator=(const CUTF8String &refString);

        CUTF32String &operator+=(const CUTF32String &refString);

        inline bool operator==(const CUTF32String &refString) const
        {
            if(this->GetLength() != refString.GetLength())
                return false;

            return MemCmp(this->pData, refString.pData, this->GetSize()) == 0;
        }

        //Methods
        CUTF32String SubString(uint32 beginOffset, uint32 length) const;
        CUTF32String ToLowercase() const;
    };

    //Non-Class Operators
    ACSTDLIB_API CUTF32String operator+(const CUTF32String &refLeft, const CUTF32String &refRight);
    ACSTDLIB_API bool operator<(const CUTF32String &refLeft, const CUTF32String &refRight);
    ACSTDLIB_API bool operator>(const CUTF32String &refLeft, const CUTF32String &refRight);
}