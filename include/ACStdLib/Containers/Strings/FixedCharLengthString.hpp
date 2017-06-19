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
#include "../../Natural.hpp"
#include "ResizeableString.hpp"

namespace ACStdLib
{
    template<typename CharType>
    class ACSTDLIB_API FixedCharLengthString : public ResizeableString<CharType>
    {
    protected:
        //Methods
        void Append(CharType c);
        void Append(const CharType *pStr, uint32 length);
        void Assign(CharType c);
        void Assign(const CharType *pStr, uint32 length);
    public:
        //Methods
        uint32 Find(CharType c, uint32 startPos = 0) const;
        int32 FindReverse(CharType c, uint32 startPos = Natural<uint32>::Max()) const;

        //Inline methods
        inline bool Contains(CharType c) const
        {
            return this->Find(c) != Natural<uint32>::Max();
        }

        //Inline Operators
        inline CharType &operator[](uint32 index)
        {
            return this->data[index];
        }

        inline const CharType operator[](uint32 index) const
        {
            return this->data[index];
        }

        //Inline
        inline uint32 GetLength() const
        {
            return this->nElements;
        }
    };
}