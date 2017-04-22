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
#include "../AInputStream.h"

namespace ACStdLib
{
    class ACSTDLIB_API CBitInputStream
    {
    private:
        //Members
        AInputStream &refInput;
        uint64 buffer;
        uint8 validBitsInBuffer;
        bool hitEnd;
        uint32 nReadBits;

        //Methods
        void EnsureBufferFilled(uint8 nBits);

    public:
        //Constructor
        CBitInputStream(AInputStream &refInput);

        //Methods
        uint64 Get(uint8 nBits);
        void Skip(uint32 nBits);

        //Inline
        inline uint32 GetNumberOfReadBits() const
        {
            return this->nReadBits;
        }
        /*
        inline uint32 GetRemainingBits() const
        {
            return this->refInput.GetRemainingBytes() * 8 + this->validBitsInBuffer;
        }
        */

        inline bool HitEnd() const
        {
            return this->hitEnd;
        }

        inline uint64 Read(uint8 nBits)
        {
            register uint64 tmp;

            tmp = this->Get(nBits);
            this->validBitsInBuffer -= nBits;
            this->nReadBits += nBits;

            return tmp;
        }
    };
}