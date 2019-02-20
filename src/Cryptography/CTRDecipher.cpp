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
#include <Std++/Cryptography/CTRDecipher.hpp>
//Local
#include <Std++/Constants.hpp>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX::Crypto;

//Public methods
bool CTRDecipher::IsAtEnd() const
{
	return this->inputStream.IsAtEnd();
}

uint32 CTRDecipher::ReadBytes(void *destination, uint32 count)
{
	uint8 buffer[c_io_blockSize];

	uint8* dest = static_cast<uint8 *>(destination);
	while(count)
	{
		//fill buffer is empty
		if(this->leftBytesInBlock == 0)
		{
			this->cipher->Encrypt(this->counter.GetKeyStreamBlock(), &this->buffer[0]);
			this->counter.Increment();
			this->leftBytesInBlock = this->counter.GetKeyStreamBlockSize();
		}

		uint8 nBytesToDec = Math::Min(this->leftBytesInBlock, static_cast<uint8>(count));
		uint32 nBytesRead = this->inputStream.ReadBytes(buffer, nBytesToDec);
		nBytesToDec = Math::Min(nBytesToDec, static_cast<const uint8 &>(nBytesRead));

		for(uint8 i = 0; i < nBytesToDec; i++)
			dest[i] = this->buffer[this->buffer.GetNumberOfElements() - this->leftBytesInBlock + i] xor buffer[i];

		dest += nBytesToDec;
		count -= nBytesToDec;
		this->leftBytesInBlock -= nBytesToDec;
	}

	return static_cast<uint32>(dest - static_cast<uint8 *>(destination));
}

uint32 CTRDecipher::Skip(uint32 nBytes)
{
	NOT_IMPLEMENTED_ERROR;
	return 0;
}
