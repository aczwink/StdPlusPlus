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
#include <Std++/Cryptography/ECBCipher.hpp>
//Local
#include <Std++/Cryptography/CipherAlgorithm.hpp>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX::Crypto;

//Public methods
void ECBCipher::Finalize()
{
	this->Flush();

	//we do pkcs padding
	uint8 left = this->cipher->GetBlockSize() - this->nBytesInBlock;
	for(uint8 i = this->nBytesInBlock; i < this->cipher->GetBlockSize(); i++)
		this->unencrypted[i] = left;
	this->nBytesInBlock += left;

	this->Flush();
}

void ECBCipher::Flush()
{
	if(this->nBytesInBlock == this->cipher->GetBlockSize()) //check if buffer is full
	{
		this->cipher->Encrypt(&this->unencrypted[0], &this->encrypted[0]);
		this->outputStream.WriteBytes(&this->encrypted[0], this->nBytesInBlock);
		this->nBytesInBlock = 0;
	}
	this->outputStream.Flush();
}

uint32 ECBCipher::WriteBytes(const void *source, uint32 size)
{
	uint8* src = (uint8 *) source;
	//encrypt and write blocks
	while(size)
	{
		if(this->nBytesInBlock == this->cipher->GetBlockSize()) //check if buffer is full
		{
			this->cipher->Encrypt(&this->unencrypted[0], &this->encrypted[0]);
			this->outputStream.WriteBytes(&this->encrypted[0], this->nBytesInBlock);
			this->nBytesInBlock = 0;
		}

		uint8 nBytesToBuffer = Math::Min(static_cast<uint8>(this->cipher->GetBlockSize() - this->nBytesInBlock), static_cast<uint8>(size));
		MemCopy(&this->unencrypted[this->nBytesInBlock], src, nBytesToBuffer);
		this->nBytesInBlock += nBytesToBuffer;
		src += nBytesToBuffer;
		size -= nBytesToBuffer;
	}
	return static_cast<uint32>(src - (uint8*)source);
}