/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Compression/SlidingDictionary.hpp>
//Local
#include <Std++/Mathematics.hpp>
#include <Std++/Memory.hpp>
//Namespaces
using namespace StdXX;

//Constructor
SlidingDictionary::SlidingDictionary(uint32 size)
{
	this->pBuffer = (byte *)MemAlloc(size);
	this->size = size;
	this->index = 0;
}

//Destructor
SlidingDictionary::~SlidingDictionary()
{
	MemFree(this->pBuffer);
}

//Public methods
void SlidingDictionary::Copy(uint16 distance, uint16 length, OutputStream &refOutput)
{
	uint32 readIndex, nBytesToWrite;

	readIndex = (this->size + this->index - distance) % this->size;
	while(length)
	{
		//check all overlaps
		nBytesToWrite = Math::Min(uint32(length), this->size - readIndex);
		nBytesToWrite = Math::Min(nBytesToWrite, this->size - this->index);
		nBytesToWrite = Math::Min(nBytesToWrite, this->index - readIndex); //we can not overtake what we didnt read yet

		//exchange bytes
		refOutput.WriteBytes(&this->pBuffer[readIndex], nBytesToWrite);
		MemCopy(&this->pBuffer[this->index], &this->pBuffer[readIndex], nBytesToWrite);

		//update
		length -= (uint16)nBytesToWrite;
		readIndex = (readIndex + nBytesToWrite) % this->size;
		this->index = (this->index + nBytesToWrite) % this->size;
	}
}