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
#include "../../../Natural.hpp"
#include "../VariableCharLengthString.hpp"
#include "CConstUTF16StringIterator.h"

namespace ACStdLib
{
    //Forward declarations
    class ByteString;
    class UTF8String;
    class CUTF32String;

    class ACSTDLIB_API UTF16String : public VariableCharLengthString<uint16>
    {
        friend class CConstUTF16StringIterator;
    private:
        //Methods
        uint32 Decode(const uint16 *pSrc, bool &refIsSurrogate) const;
        bool Encode(uint32 codePoint, uint16 *pDest); //return true if surrogate pair was encoded, false otherwise
    public:
        //Constructors
        inline UTF16String()
        {
        }

        inline UTF16String(const UTF16String &refString) //copy ctor
        {
            *this = refString;
        }

        inline UTF16String(UTF16String &&refString) //move ctor
        {
            *this = refString;
        }

        inline UTF16String(const uint16 *pStr)
        {
            *this = pStr;
        }

        inline UTF16String(const ByteString &refString)
        {
            *this = refString;
        }

        inline UTF16String(const UTF8String &refString)
        {
            *this = refString;
        }

        inline UTF16String(const CUTF32String &refString)
        {
            *this = refString;
        }

        UTF16String(const uint16 *pString, uint32 length);

        //Operators
        UTF16String &operator=(const UTF16String &refString); //copy assign
        UTF16String &operator=(UTF16String &&refString); //move assign
        UTF16String &operator=(const uint16 *pStr);
        UTF16String &operator=(const ByteString &refString);
        UTF16String &operator=(const UTF8String &refString);
        UTF16String &operator=(const CUTF32String &refString);
        UTF16String operator+(const UTF16String &refRight) const;
        UTF16String &operator+=(uint16 c);
        UTF16String &operator+=(const ByteString &refString);
        UTF16String &operator+=(const UTF16String &refString);
        bool operator<(const UTF16String &refRight) const;
        bool operator>(const UTF16String &refRight) const;

        //Inline operators
        inline bool operator==(const UTF16String &refString) const
        {
            if(this->GetLength() != refString.GetLength())
                return false;

            return MemCmp(this->data, refString.data, this->GetSize()) == 0;
        }

        //Methods
        bool Contains(uint32 codePoint) const;
        uint32 Find(uint16 c, uint32 startPos = 0) const;
        uint32 FindReverse(uint16 c, uint32 startPos = Natural<uint32>::Max()) const;
        UTF16String ToLowercase() const;

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