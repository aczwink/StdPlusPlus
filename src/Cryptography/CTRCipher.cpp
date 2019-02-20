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
#include <Std++/Cryptography/CTRCipher.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX::Crypto;

//Public methods
void CTRCipher::Flush()
{
	//this stream does not buffer
}

uint32 CTRCipher::WriteBytes(const void *source, uint32 size)
{
	const uint8* src = static_cast<const uint8 *>(source);
	while(size)
	{
		//how many bytes can still be encrypted with current keystream block
		if(this->leftBytesInBlock == 0)
		{
			this->cipher->Encrypt(this->counter.GetKeyStreamBlock(), &this->buffer[0]);
			this->counter.Increment();
			this->leftBytesInBlock = this->counter.GetKeyStreamBlockSize();
		}

		uint8 nBytesToEnc = Math::Min(this->leftBytesInBlock, static_cast<uint8>(size));
		for(uint8 i = 0; i < nBytesToEnc; i++)
			this->buffer[this->buffer.GetNumberOfElements() - this->leftBytesInBlock + i] = this->buffer[this->buffer.GetNumberOfElements() - this->leftBytesInBlock + i] xor src[i];
		this->outputStream.WriteBytes(&this->buffer[this->buffer.GetNumberOfElements() - this->leftBytesInBlock], nBytesToEnc);

		src += nBytesToEnc;
		size -= nBytesToEnc;
		this->leftBytesInBlock -= nBytesToEnc;
	}

	return static_cast<uint32>(src - static_cast<const uint8 *>(source));
}
