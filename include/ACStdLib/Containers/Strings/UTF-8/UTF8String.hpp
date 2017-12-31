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
#include "../VariableCharLengthString.hpp"
#include "ConstUTF8StringIterator.hpp"

namespace ACStdLib
{
    //Forward declarations
    class ByteString;
    class UTF16String;
    class UTF32String;

    class ACSTDLIB_API UTF8String : public VariableCharLengthString<byte>
    {
        friend class ConstUTF8StringIterator;
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

        inline UTF8String(const char *string)
        {
            *this = string;
        }

        inline UTF8String(const uint16 *pString)
        {
            *this = pString;
        }

        inline UTF8String(const ByteString &refString)
        {
            *this = refString;
        }

        inline UTF8String(const UTF16String &refString)
        {
            *this = refString;
        }

        inline UTF8String(const UTF32String &refString)
        {
            *this = refString;
        }

        //Operators
        UTF8String &operator=(const UTF8String &refString); //copy assign
        UTF8String &operator=(UTF8String &&refString); //move assign
        /**
         * We assume that @p string is already UTF-8 encoded.
         *
         * @param string
         * @return
         */
        UTF8String &operator=(const char *string);
        UTF8String &operator=(const uint16 *pString);
        UTF8String &operator=(const ByteString &refString);
        UTF8String &operator=(const UTF16String &refString);
        UTF8String &operator=(const UTF32String &refString);
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
        inline const ConstUTF8StringIterator begin() const
        {
            return ConstUTF8StringIterator(*this);
        }

        inline const ConstUTF8StringIterator end() const
        {
            return ConstUTF8StringIterator(*this, this->GetNumberOfElements());
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