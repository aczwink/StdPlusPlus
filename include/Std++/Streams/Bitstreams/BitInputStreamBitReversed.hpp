/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include "../InputStream.hpp"

namespace StdXX
{
    /**
     * This class reads bits from low to high significance i.e. the first bit read is the LSB of the first byte.
     */
    class STDPLUSPLUS_API BitInputStreamBitReversed
    {
    private:
        //Members
        InputStream &refInput;
        uint64 buffer;
        uint8 validBitsInBuffer;
        bool hitEnd;
        uint32 nReadBits;

        //Methods
        void EnsureBufferFilled(uint8 nBits);

    public:
        //Constructor
        BitInputStreamBitReversed(InputStream &refInput);

        //Methods
        uint64 Get(uint8 nBits);
        void Skip(uint32 nBits);

        //Inline
        inline uint32 GetReadBitsCount() const
        {
            return this->nReadBits;
        }

        inline uint64 Read(uint8 nBits)
        {
            uint64 tmp = this->Get(nBits);
            this->validBitsInBuffer -= nBits;
            this->nReadBits += nBits;
            this->buffer >>= nBits;

            return tmp;
        }
    };
}