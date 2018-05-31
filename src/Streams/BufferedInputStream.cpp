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
//Class Header
#include <Std++/Streams/BufferedInputStream.hpp>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Memory.h>
//Namespaces
using namespace StdPlusPlus;

//Constructor
BufferedInputStream::BufferedInputStream(InputStream &refInputStream, uint32 bufferSize) : refInput(refInputStream)
{
	this->buffer = (byte *)MemAlloc(bufferSize);
	this->pEnd = this->buffer + bufferSize;
	this->current = this->pEnd;
}

//Destructor
BufferedInputStream::~BufferedInputStream()
{
	MemFree(this->buffer);
}

//Private methods
void BufferedInputStream::FillBufferIfEmpty()
{
	if(this->current == this->pEnd)
	{
		this->pEnd = this->buffer + this->refInput.ReadBytes(this->buffer, uint32(this->pEnd - this->buffer));
		this->current = this->buffer;
	}
}

//Public methods
bool BufferedInputStream::IsAtEnd() const
{
	return this->current == this->pEnd && this->refInput.IsAtEnd();
}

uint32 BufferedInputStream::ReadBytes(void *destination, uint32 count)
{
	byte *byteDestination = (byte *)destination;
	uint32 nReadBytes = 0;
	while(count)
	{
		this->FillBufferIfEmpty();
		if(this->current == this->pEnd)
			return nReadBytes;

		uint32 size = uint32(this->pEnd - this->current);
		if(size > count)
			size = count;

		MemCopy(byteDestination, this->current, size);
		this->current += size;
		byteDestination += size;
		count -= size;
		nReadBytes += size;
	}

	return nReadBytes;
}

uint32 BufferedInputStream::Skip(uint32 nBytes)
{
	uint32 size, nBytesSkipped;

	nBytesSkipped = 0;
	while(nBytes)
	{
		this->FillBufferIfEmpty();
		if(this->current == this->pEnd)
			break;

		size = uint32(this->pEnd - this->current);
		if(size > nBytes)
			size = nBytes;

		this->current += size;
		nBytes -= size;

		nBytesSkipped += size;
	}

	return nBytesSkipped;
}