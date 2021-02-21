/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
//Class Header
#include <Std++/Streams/BufferInputStream.hpp>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Memory.hpp>
//Namespaces
using namespace StdXX;

//Constructor
BufferInputStream::BufferInputStream(const void *pBuffer, uint32 size)
{
    this->pStart = static_cast<const byte *>(pBuffer);
    this->current = this->pStart;
    this->endPos = this->pStart + size;
}

//Public methods
uint32 BufferInputStream::GetBytesAvailable() const
{
	return static_cast<uint32>(this->endPos - this->current);
}

uint64 BufferInputStream::QueryCurrentOffset() const
{
    return this->current - this->pStart;
}

uint64 BufferInputStream::QueryRemainingBytes() const
{
    return this->endPos - this->current;
}

uint64 BufferInputStream::QuerySize() const
{
    return this->endPos - this->pStart;
}

bool BufferInputStream::IsAtEnd() const
{
	return this->current == this->endPos;
}

uint32 BufferInputStream::ReadBytes(void *pDestination, uint32 count)
{
    if(count > this->QueryRemainingBytes())
    {
        //trying to read more than there is
        count = (uint32) this->QueryRemainingBytes();
    }

    MemCopy(pDestination, this->current, count);
    this->current += count;

    return count;
}

void BufferInputStream::SeekTo(uint64 offset)
{
    ASSERT(this->pStart + offset <= this->endPos, "If you see this, report to StdXX");

    this->current = this->pStart + offset;
}

uint32 BufferInputStream::Skip(uint32 nBytes)
{
    if(nBytes > this->QueryRemainingBytes())
    {
        //trying to read more than there is
        nBytes = (uint32) this->QueryRemainingBytes();
    }

    this->current += nBytes;

    return nBytes;
}