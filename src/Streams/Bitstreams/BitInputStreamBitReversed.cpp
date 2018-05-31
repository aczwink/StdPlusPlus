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
//Class header
#include <Std++/Streams/Bitstreams/BitInputStreamBitReversed.hpp>
//Local
#include <Std++/Debug.hpp>
//Namespaces
using namespace StdPlusPlus;

//Constructor
BitInputStreamBitReversed::BitInputStreamBitReversed(InputStream &refInput) : refInput(refInput)
{
	this->buffer = 0;
	this->validBitsInBuffer = 0;
	this->hitEnd = false;
	this->nReadBits = 0;
}

//Public methods
uint64 BitInputStreamBitReversed::Get(uint8 nBits)
{
	ASSERT(nBits <= 64, "If you see this, report to StdPlusPlus");

	this->EnsureBufferFilled(nBits);

	return this->buffer & ((1 << nBits) - 1);
}

void BitInputStreamBitReversed::Skip(uint32 nBits)
{
	register uint8 left;

	while(nBits)
	{
		left = MIN(nBits, 8);
		this->EnsureBufferFilled(left);
		this->validBitsInBuffer -= left;
		this->nReadBits += left;
		this->buffer >>= nBits;

		nBits -= left;

		if(this->hitEnd)
			break;
	}
}

//Private methods
void BitInputStreamBitReversed::EnsureBufferFilled(uint8 nBits)
{
	register byte nextByte;

#ifdef _DEBUG
	if(nBits > 64)
	{
		NOT_IMPLEMENTED_ERROR;
	}
#endif

	while(this->validBitsInBuffer < nBits)
	{
		ASSERT(this->validBitsInBuffer + 8 <= 64, "If you see this, report to StdPlusPlus");

		if(this->refInput.IsAtEnd())
		{
			this->hitEnd = true;
			break;
		}

		this->refInput.ReadBytes(&nextByte, 1);
		this->buffer |= (((uint16)nextByte) << this->validBitsInBuffer);
		this->validBitsInBuffer += 8;
	}
}