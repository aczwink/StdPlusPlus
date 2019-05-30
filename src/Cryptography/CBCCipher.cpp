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
#include <Std++/Cryptography/CBCCipher.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX::Crypto;

//Public methods
void CBCCipher::Finalize()
{
	this->FlushBufferIfFull();

	//we do pkcs padding
	uint8 left = this->cipher->GetBlockSize() - this->nBytesInBlock;
	for(uint8 i = this->nBytesInBlock; i < this->cipher->GetBlockSize(); i++)
		this->unencrypted[i] = left;
	this->nBytesInBlock = this->cipher->GetBlockSize();

	this->Flush();
}

void CBCCipher::Flush()
{
	this->FlushBufferIfFull();

	this->outputStream.Flush();
}

uint32 CBCCipher::WriteBytes(const void *source, uint32 size)
{
	uint8* src = (uint8 *) source;
	//encrypt and write blocks
	while(size)
	{
		this->FlushBufferIfFull();

		uint32 nBytesToBuffer = Math::Min(static_cast<uint32>(this->cipher->GetBlockSize() - this->nBytesInBlock), size);
		MemCopy(&this->unencrypted[this->nBytesInBlock], src, nBytesToBuffer);
		this->nBytesInBlock += nBytesToBuffer;
		src += nBytesToBuffer;
		size -= nBytesToBuffer;
	}
	return static_cast<uint32>(src - (uint8*)source);
}

//Private methods
void CBCCipher::FlushBufferIfFull()
{
	if(this->nBytesInBlock == this->cipher->GetBlockSize()) //check if buffer is full
	{
		for(uint8 i = 0; i < this->nBytesInBlock; i++)
			this->unencrypted[i] = this->unencrypted[i] xor this->lastBlock[i];

		this->cipher->Encrypt(&this->unencrypted[0], this->currentBlock);
		this->outputStream.WriteBytes(this->currentBlock, this->nBytesInBlock);
		Swap(this->currentBlock, this->lastBlock);
		this->nBytesInBlock = 0;
	}
}