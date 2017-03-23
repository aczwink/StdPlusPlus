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
//Class Header
#include "../../headers/Streams/CBufferInputStream.h"
//Local
#include "../../headers/Debug.h"
#include "../../headers/Memory.h"
//Namespaces
using namespace ACStdLib;

//Constructor
CBufferInputStream::CBufferInputStream(const byte *pBuffer, uint32 size)
{
    this->pStart = pBuffer;
    this->pCurrent = this->pStart;
    this->pEnd = this->pStart + size;
    this->hitEnd = false;
}

//Public Functions
uint64 CBufferInputStream::GetCurrentOffset() const
{
    return this->pCurrent - this->pStart;
}

uint64 CBufferInputStream::GetRemainingBytes() const
{
    return this->pEnd - this->pCurrent;
}

uint64 CBufferInputStream::GetSize() const
{
    return this->pEnd - this->pStart;
}

bool CBufferInputStream::HitEnd() const
{
    return this->hitEnd;
}

byte CBufferInputStream::ReadByte()
{
    if(this->pCurrent == this->pEnd)
    {
        this->hitEnd = true;
        return 0xFF;
    }

    return *this->pCurrent++;
}

uint32 CBufferInputStream::ReadBytes(void *pDestination, uint32 count)
{
    if(count > this->GetRemainingBytes())
    {
        //trying to read more than there is
        this->hitEnd = true;
        count = (uint32)this->GetRemainingBytes();
    }

    MemCopy(pDestination, this->pCurrent, count);
    this->pCurrent += count;

    return count;
}

void CBufferInputStream::SetCurrentOffset(uint64 offset)
{
    ASSERT(this->pStart + offset <= this->pEnd);

    this->pCurrent = this->pStart + offset;
}

uint32 CBufferInputStream::Skip(uint32 nBytes)
{
    if(nBytes > this->GetRemainingBytes())
    {
        //trying to read more than there is
        this->hitEnd = true;
        nBytes = (uint32)this->GetRemainingBytes();
    }

    this->pCurrent += nBytes;

    return nBytes;
}