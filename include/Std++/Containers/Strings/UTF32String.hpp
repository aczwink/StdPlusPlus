/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include "FixedCharLengthString.hpp"

namespace StdXX
{
    class STDPLUSPLUS_API UTF32String : public FixedCharLengthString<uint32>
    {
    public:
        //Constructors
        inline UTF32String()
        {
        }

        inline UTF32String(const UTF32String &refString) //copy ctor
        {
            *this = refString;
        }

        inline UTF32String(UTF32String &&refString) //move ctor
        {
            *this = refString;
        }

        inline UTF32String(char c)
        {
            *this = (uint8)c;
        }

        inline UTF32String(const char *pString)
        {
            *this = (const uint8 *)pString;
        }

        inline UTF32String(const uint8 *pString)
        {
            *this = pString;
        }

        inline UTF32String(const uint16 *pStr)
        {
            *this = pStr;
        }

        //Operators
        UTF32String &operator=(const UTF32String &refString); //copy assign
        UTF32String &operator=(uint8 c);
        UTF32String &operator=(const uint8 *pString);
        UTF32String &operator=(const uint16 *pStr);

        UTF32String &operator+=(const UTF32String &refString);

        inline bool operator==(const UTF32String &refString) const
        {
            if(this->GetLength() != refString.GetLength())
                return false;

            return MemCmp(this->data, refString.data, this->GetSize()) == 0;
        }

        //Methods
        UTF32String SubString(uint32 beginOffset, uint32 length) const;
        UTF32String ToLowercase() const;
    };

    //Non-Class Operators
    STDPLUSPLUS_API UTF32String operator+(const UTF32String &refLeft, const UTF32String &refRight);
    STDPLUSPLUS_API bool operator<(const UTF32String &refLeft, const UTF32String &refRight);
    STDPLUSPLUS_API bool operator>(const UTF32String &refLeft, const UTF32String &refRight);
}