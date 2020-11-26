/*
* Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Buffers/RingBuffer.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;

//Public methods
void RingBuffer::Read(void* destination, uint16 distance, uint16 length) const
{
	uint8* dest = static_cast<uint8 *>(destination);
	while (length)
	{
		uint32 readIndex = this->CalcBackOffsetIndex(distance);
		uint32 nBytesToWrite = Math::Min(uint32(length), this->CalcBackOffsetIndex(readIndex));
		MemCopy(dest, &this->data[readIndex], nBytesToWrite);

		//update
		distance -= (uint16)nBytesToWrite;
		length -= (uint16)nBytesToWrite;
	}
}