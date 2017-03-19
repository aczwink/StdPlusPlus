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
#include "../AResizeableContainer.h"

namespace ACStdLib
{
    template<typename CharType>
    class ACSTDLIB_API AVariableCharLengthString : public AResizeableContainer<CharType>
    {
    protected:
        //Members
        uint32 length;
    public:
        //Constructor
        inline AVariableCharLengthString()
        {
            this->length = 0;
            this->SetAllocationInterval(16);
        }

        //Inline
        inline void EnsureCapacity(uint32 requiredNumberOfElements)
        {
            requiredNumberOfElements++; //null char
            AResizeableContainer<CharType>::EnsureCapacity(requiredNumberOfElements);
        }

        inline const CharType *GetC_Str() const
        {
            return this->pData;
        }

        inline uint32 GetLength() const
        {
            return this->length;
        }

        inline void Release()
        {
            this->length = 0;

            AResizeableContainer<CharType>::Release();
        }
    };
}