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
#include "../AVariableCharLengthString.h"
#include "CConstUTF16StringIterator.h"

namespace ACStdLib
{
    //Forward declarations
    class C8BitString;
    class UTF8String;
    class CUTF32String;

    class ACSTDLIB_API CUTF16String : public AVariableCharLengthString<uint16>
    {
        friend class CConstUTF16StringIterator;
    private:
        //Methods
        uint32 Decode(const uint16 *pSrc, bool &refIsSurrogate) const;
        bool Encode(uint32 codePoint, uint16 *pDest); //return true if surrogate pair was encoded, false otherwise
    public:
        //Constructors
        inline CUTF16String()
        {
        }

        inline CUTF16String(const CUTF16String &refString) //copy ctor
        {
            *this = refString;
        }

        inline CUTF16String(CUTF16String &&refString) //move ctor
        {
            *this = refString;
        }

        inline CUTF16String(const char *pString)
        {
            *this = pString;
        }

        inline CUTF16String(const uint16 *pStr)
        {
            *this = pStr;
        }

        inline CUTF16String(const C8BitString &refString)
        {
            *this = refString;
        }

        inline CUTF16String(const UTF8String &refString)
        {
            *this = refString;
        }

        inline CUTF16String(const CUTF32String &refString)
        {
            *this = refString;
        }

        CUTF16String(const uint16 *pString, uint32 length);

        //Operators
        CUTF16String &operator=(const CUTF16String &refString); //copy assign
        CUTF16String &operator=(CUTF16String &&refString); //move assign
        CUTF16String &operator=(const char *pStr);
        CUTF16String &operator=(const uint16 *pStr);
        CUTF16String &operator=(const C8BitString &refString);
        CUTF16String &operator=(const UTF8String &refString);
        CUTF16String &operator=(const CUTF32String &refString);
        CUTF16String operator+(const CUTF16String &refRight) const;
        CUTF16String &operator+=(uint16 c);
        CUTF16String &operator+=(const C8BitString &refString);
        CUTF16String &operator+=(const CUTF16String &refString);
        bool operator<(const CUTF16String &refRight) const;
        bool operator>(const CUTF16String &refRight) const;

        //Inline operators
        inline bool operator==(const CUTF16String &refString) const
        {
            if(this->GetLength() != refString.GetLength())
                return false;

            return MemCmp(this->pData, refString.pData, this->GetSize()) == 0;
        }

        //Methods
        bool Contains(uint32 codePoint) const;
        uint32 Find(uint16 c, uint32 startPos = 0) const;
        uint32 FindReverse(uint16 c, uint32 startPos = UINT32_MAX) const;
        CUTF16String ToLowercase() const;

        //For range-based loop
        inline const CConstUTF16StringIterator begin() const
        {
            return CConstUTF16StringIterator(*this);
        }

        inline const CConstUTF16StringIterator end() const
        {
            return CConstUTF16StringIterator(*this, this->GetNumberOfElements());
        }
    };
}