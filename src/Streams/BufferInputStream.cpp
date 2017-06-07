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
#include <ACStdLib/Streams/BufferInputStream.hpp>
//Local
#include <ACStdLib/Debug.h>
#include <ACStdLib/Memory.h>
//Namespaces
using namespace ACStdLib;

//Constructor
BufferInputStream::BufferInputStream(const byte *pBuffer, uint32 size)
{
    this->pStart = pBuffer;
    this->current = this->pStart;
    this->endPos = this->pStart + size;
}

//Public Functions
uint64 BufferInputStream::GetCurrentOffset() const
{
    return this->current - this->pStart;
}

uint64 BufferInputStream::GetRemainingBytes() const
{
    return this->endPos - this->current;
}

uint64 BufferInputStream::GetSize() const
{
    return this->endPos - this->pStart;
}

bool BufferInputStream::IsAtEnd() const
{
	return this->current == this->endPos;
}

uint32 BufferInputStream::ReadBytes(void *pDestination, uint32 count)
{
    if(count > this->GetRemainingBytes())
    {
        //trying to read more than there is
        count = (uint32)this->GetRemainingBytes();
    }

    MemCopy(pDestination, this->current, count);
    this->current += count;

    return count;
}

void BufferInputStream::SetCurrentOffset(uint64 offset)
{
    ASSERT(this->pStart + offset <= this->endPos);

    this->current = this->pStart + offset;
}

uint32 BufferInputStream::Skip(uint32 nBytes)
{
    if(nBytes > this->GetRemainingBytes())
    {
        //trying to read more than there is
        nBytes = (uint32)this->GetRemainingBytes();
    }

    this->current += nBytes;

    return nBytes;
}