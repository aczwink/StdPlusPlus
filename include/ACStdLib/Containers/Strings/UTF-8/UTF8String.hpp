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
#include <ACStdLib/Containers/Strings/AVariableCharLengthString.h>
#include "CConstUTF8StringIterator.h"

namespace ACStdLib
{
    //Forward declarations
    class C8BitString;
    class CUTF16String;
    class CUTF32String;

    class ACSTDLIB_API UTF8String : public AVariableCharLengthString<byte>
    {
        friend class CConstUTF8StringIterator;
    private:
        //Methods
        uint32 Decode(const byte *pSrc, uint8 &refNBytes) const;
        uint8 Encode(uint32 codePoint, byte *pDest) const;
    public:
        //Constructors
        inline UTF8String()
        {
        }

        inline UTF8String(const UTF8String &refString) //copy ctor
        {
            *this = refString;
        }

        inline UTF8String(UTF8String &&refString) //move ctor
        {
            *this = refString;
        }

        inline UTF8String(const char *pString)
        {
            *this = pString;
        }

        inline UTF8String(const uint16 *pString)
        {
            *this = pString;
        }

        inline UTF8String(const C8BitString &refString)
        {
            *this = refString;
        }

        inline UTF8String(const CUTF16String &refString)
        {
            *this = refString;
        }

        inline UTF8String(const CUTF32String &refString)
        {
            *this = refString;
        }

        //Operators
        UTF8String &operator=(const UTF8String &refString); //copy assign
        UTF8String &operator=(UTF8String &&refString); //move assign
        UTF8String &operator=(const char *pString);
        UTF8String &operator=(const uint16 *pString);
        UTF8String &operator=(const C8BitString &refString);
        UTF8String &operator=(const CUTF16String &refString);
        UTF8String &operator=(const CUTF32String &refString);
        UTF8String &operator+=(uint32 codePoint);
        UTF8String &operator+=(const UTF8String &refString);

        //Inline operators
        inline bool operator==(const UTF8String &refRight)
        {
            if(this->GetLength() != refRight.GetLength())
                return false;

            return MemCmp(this->GetC_Str(), refRight.GetC_Str(), this->GetNumberOfElements()) == 0;
        }

        //For range-based loop
        inline const CConstUTF8StringIterator begin() const
        {
            return CConstUTF8StringIterator(*this);
        }

        inline const CConstUTF8StringIterator end() const
        {
            return CConstUTF8StringIterator(*this, this->GetNumberOfElements());
        }
    };

    //Binary operators
    inline UTF8String operator+(const UTF8String &refLeft, const UTF8String &refRight)
    {
        UTF8String buffer(refLeft);

        buffer += refRight;

        return buffer;
    }
}