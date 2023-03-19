/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/Bitstreams/BitInputStream.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;

//Constructor
BitInputStream::BitInputStream(InputStream& refInput) : refInput(refInput)
{
	this->buffer = 0;
	this->validBitsInBuffer = 0;
	this->nReadBits = 0;
}

//Public methods
uint64 BitInputStream::Get(uint8 nBits)
{
	ASSERT(nBits <= 64, u8"can't read more than 64 bits into uint64");

	this->EnsureBufferFilled(nBits);

	uint64 result = (this->buffer >> (this->validBitsInBuffer - nBits)) & (((uint64)1 << nBits) - 1);
	return result;
}

//Private methods
void BitInputStream::EnsureBufferFilled(uint8 nBits)
{
	while(this->validBitsInBuffer < nBits)
	{
		ASSERT(this->validBitsInBuffer + 8 <= 64, u8"skipping bits in the buffer is not implemented");

		uint8 nextByte;
		uint32 nBytesRead = this->refInput.ReadBytes(&nextByte, 1);
		if(nBytesRead == 0)
			break;

		this->buffer = (this->buffer << 8) | nextByte;
		this->validBitsInBuffer += 8;
	}
}

void BitInputStream::Skip(uint32 nBits)
{
	while(nBits and !this->IsAtEnd())
	{
		uint8 left = static_cast<uint8>(Math::Min(nBits, 8_u32));
		this->EnsureBufferFilled(left);
		this->validBitsInBuffer -= left;
		this->nReadBits += left;
		nBits -= left;
	}
}
