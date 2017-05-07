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
#include "../DynamicArray/DynamicArray.hpp"
#include "../LinkedList/LinkedList.hpp"
//#include "CWString.h"

namespace ACStdLib
{
    //May contain ASCII, Latin1 or anything that fits within 8 bits
    class ACSTDLIB_API C8BitString : public AFixedCharLengthString<char>
    {
    public:
        //Constructors
        inline C8BitString()
        {
        }

        inline C8BitString(char c)
        {
            *this = c;
        }

        inline C8BitString(const char *pStr)
        {
            *this = pStr;
        }

        inline C8BitString(const char *pStr, uint32 length)
        {
            AFixedCharLengthString<char>::Assign(pStr, length);
        }

        inline C8BitString(const C8BitString &refString) //copy ctor
        {
            *this = refString;
        }

        inline C8BitString(C8BitString &&refString) //move ctor
        {
            *this = refString;
        }

        //Operators
        C8BitString &operator=(const char *pStr);
        C8BitString &operator=(C8BitString &&refString); //move assign
        C8BitString &operator+=(const C8BitString &refString);

        //Inline Operators
        inline C8BitString &operator=(char c)
        {
            AFixedCharLengthString<char>::Assign(c);

            return *this;
        }

        inline C8BitString &operator=(const C8BitString &refString) //copy assign
        {
            AFixedCharLengthString<char>::Assign(refString.pData, refString.GetLength());

            return *this;
        }

        inline C8BitString &operator+=(char c)
        {
            this->Append(c);

            return *this;
        }

        //Methods
        using AFixedCharLengthString<char>::Find;
        int32 Find(const C8BitString &refSearch, uint32 startPos = 0) const;
        using AFixedCharLengthString<char>::FindReverse;
        int32 FindReverse(const C8BitString &refSearch, uint32 startPos = UINT32_MAX) const;
        void Replace(const C8BitString &refSearch, const C8BitString &refReplace);
        void Resize(uint32 newLength);
        LinkedList<C8BitString> Split(const C8BitString &refDelimiter) const;
        C8BitString SubString(uint32 beginOffset, uint32 length) const;
        C8BitString ToLowercase() const;
        C8BitString ToUppercase() const;
    };

    ACSTDLIB_API C8BitString operator+(const C8BitString &left, const C8BitString &right);
    ACSTDLIB_API bool operator==(const C8BitString &left, const C8BitString &right);
    ACSTDLIB_API bool operator!=(const C8BitString &left, const C8BitString &right);
    ACSTDLIB_API bool operator<(const C8BitString &left, const C8BitString &right);
    ACSTDLIB_API bool operator>(const C8BitString &left, const C8BitString &right);
}