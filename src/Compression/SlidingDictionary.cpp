/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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

//Public methods
void SlidingDictionary::Copy(uint16 distance, uint16 length, OutputStream &refOutput)
{
	while(length)
	{
		uint32 readIndex = this->CalcBackOffsetIndex(distance);
		uint32 nBytesToWrite = Math::Min(uint32(length), this->CalcNumberOfBytesToRead(readIndex));

		//exchange bytes
		refOutput.WriteBytes(&this->Data()[readIndex], nBytesToWrite);
		this->CopyToTail(readIndex, nBytesToWrite);

		//update
		length -= (uint16)nBytesToWrite;
	}
}

void SlidingDictionary::CopyToTail(uint16 distance, uint16 length)
{
	while(length)
	{
		uint32 readIndex = this->CalcBackOffsetIndex(distance);
		uint32 nBytesToWrite = Math::Min(uint32(length), this->CalcNumberOfBytesToRead(readIndex));

		//exchange bytes
		RingBuffer::CopyToTail(readIndex, nBytesToWrite);

		//update
		length -= (uint16)nBytesToWrite;
	}
}
