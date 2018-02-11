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
#include "FixedCharLengthString.hpp"
#include "../Array/DynamicArray.hpp"
#include "../LinkedList/LinkedList.hpp"
//#include "CWString.h"

namespace ACStdLib
{
    //May contain ASCII, Latin1 or anything that fits within 8 bits
    class ACSTDLIB_API ByteString : public FixedCharLengthString<char>
    {
    public:
        //Constructors
        inline ByteString()
        {
        }

        inline ByteString(char c)
        {
            *this = c;
        }

        inline ByteString(const char *pStr)
        {
            *this = pStr;
        }

        inline ByteString(const char *pStr, uint32 length)
        {
            FixedCharLengthString<char>::Assign(pStr, length);
        }

        inline ByteString(const ByteString &refString) //copy ctor
        {
            *this = refString;
        }

        inline ByteString(ByteString &&refString) //move ctor
        {
            *this = refString;
        }

        //Operators
        ByteString &operator=(const char *pStr);
        ByteString &operator=(ByteString &&refString); //move assign
        ByteString &operator+=(const ByteString &refString);

        //Inline Operators
        inline ByteString &operator=(char c)
        {
            FixedCharLengthString<char>::Assign(c);

            return *this;
        }

        inline ByteString &operator=(const ByteString &refString) //copy assign
        {
            FixedCharLengthString<char>::Assign(refString.data, refString.GetLength());

            return *this;
        }

        inline ByteString &operator+=(char c)
        {
            this->Append(c);

            return *this;
        }

        //Methods
        using FixedCharLengthString<char>::Find;
        int32 Find(const ByteString &refSearch, uint32 startPos = 0) const;
        using FixedCharLengthString<char>::FindReverse;
        int32 FindReverse(const ByteString &refSearch, uint32 startPos = Natural<uint32>::Max()) const;
        void Resize(uint32 newLength);
        LinkedList<ByteString> Split(const ByteString &refDelimiter) const;
        ByteString SubString(uint32 beginOffset, uint32 length) const;
        ByteString ToLowercase() const;
        ByteString ToUppercase() const;
    };

    ACSTDLIB_API ByteString operator+(const ByteString &left, const ByteString &right);
    ACSTDLIB_API bool operator==(const ByteString &left, const ByteString &right);
    ACSTDLIB_API bool operator!=(const ByteString &left, const ByteString &right);
    ACSTDLIB_API bool operator<(const ByteString &left, const ByteString &right);
    ACSTDLIB_API bool operator>(const ByteString &left, const ByteString &right);
}