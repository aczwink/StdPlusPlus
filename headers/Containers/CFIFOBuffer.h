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
#include "../Streams/AInputStream.h"
#include "../Streams/AOutputStream.h"
#include "AResizeableContainer.h"

namespace ACStdLib
{
    class ACSTDLIB_API CFIFOBuffer : public AResizeableContainer<byte>, public AInputStream, public AOutputStream
    {
    private:
        //Members
        byte *pCurrentFront;
        byte *pCurrentTail;
        bool hitEnd;

        //Methods
        void EnsureStorage(uint32 requiredAdditionalSize);

    public:
        //Constructor
        CFIFOBuffer();

        //Methods
        void EnsureCapacity(uint32 requiredNumberOfElements);
        bool HitEnd() const;
        byte ReadByte();
        uint32 ReadBytes(void *pDestination, uint32 count);
        void Release();
        uint32 Skip(uint32 nBytes);
        void WriteByte(byte b);
        uint32 WriteBytes(const void *pSource, uint32 count);

        //Inline
        inline uint32 GetRemainingBytes() const
        {
            return (uint32)(this->pCurrentTail - this->pCurrentFront);
        }
    };
}