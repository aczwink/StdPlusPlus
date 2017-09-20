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
#include <ACStdLib/Streams/BufferedInputStream.hpp>
//Local
#include <ACStdLib/Debug.h>
#include <ACStdLib/Memory.h>
//Namespaces
using namespace ACStdLib;

//Constructor
BufferedInputStream::BufferedInputStream(InputStream &refInputStream, uint32 bufferSize) : refInput(refInputStream)
{
	this->pBuffer = (byte *)MemAlloc(bufferSize);
	this->pEnd = this->pBuffer + bufferSize;
	this->pCurrent = this->pEnd;
}

//Destructor
BufferedInputStream::~BufferedInputStream()
{
	MemFree(this->pBuffer);
}

//Private methods
void BufferedInputStream::FillBufferIfEmpty()
{
	if(this->pCurrent == this->pEnd)
	{
		this->pEnd = this->pBuffer + this->refInput.ReadBytes(this->pBuffer, uint32(this->pEnd - this->pBuffer));
		this->pCurrent = this->pBuffer;
	}
}

//Public methods
bool BufferedInputStream::IsAtEnd() const
{
	return this->pCurrent == this->pEnd && this->refInput.IsAtEnd();
}

byte BufferedInputStream::ReadByte()
{
	this->FillBufferIfEmpty();
	if(this->pCurrent == this->pEnd)
		return -1;

	return *this->pCurrent++;
}

uint32 BufferedInputStream::ReadBytes(void *pDestination, uint32 count)
{
	uint32 size, nReadBytes;
	byte *pDest;

	pDest = (byte *)pDestination;
	nReadBytes = 0;

	while(count)
	{
		this->FillBufferIfEmpty();
		if(this->pCurrent == this->pEnd)
			return nReadBytes;

		size = uint32(this->pEnd - this->pCurrent);
		if(size > count)
			size = count;

		MemCopy(pDestination, this->pCurrent, size);
		this->pCurrent += size;
		pDest += size;
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
		if(this->pCurrent == this->pEnd)
			break;

		size = uint32(this->pEnd - this->pCurrent);
		if(size > nBytes)
			size = nBytes;

		this->pCurrent += size;
		nBytes -= size;

		nBytesSkipped += size;
	}

	return nBytesSkipped;
}