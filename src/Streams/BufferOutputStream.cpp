/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/Streams/BufferOutputStream.hpp>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Memory.hpp>
//Namespaces
using namespace StdXX;

//Constructor
BufferOutputStream::BufferOutputStream(void *pBuffer, uint32 size)
{
	this->pStart = (byte *)pBuffer;
	this->pCurrent = this->pStart;
	this->pEnd = this->pStart + size;
	this->hitEnd = false;
}

//Public methods
void BufferOutputStream::Flush()
{
	//this stream always flushes
}

uint64 BufferOutputStream::GetCurrentOffset() const
{
	return this->pCurrent - this->pStart;
}

void BufferOutputStream::SetCurrentOffset(uint64 offset)
{
	ASSERT(this->pStart + offset < this->pEnd, u8"Offset out of bounds.");

	this->pCurrent = this->pStart + offset;
}

uint32 BufferOutputStream::WriteBytes(const void *pSource, uint32 count)
{
	ASSERT(pSource, u8"Can't write from null pointer.");
	ASSERT(this->pCurrent + count <= this->pEnd, u8"Not enough space in buffer to write the data.");

	MemCopy(this->pCurrent, pSource, count);
	this->pCurrent += count;

	return count;
}