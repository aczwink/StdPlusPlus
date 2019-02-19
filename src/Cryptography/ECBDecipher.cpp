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
#include <Std++/Cryptography/ECBDecipher.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX::Crypto;

//Public methods
bool ECBDecipher::IsAtEnd() const
{
	return this->inputStream.IsAtEnd() && (this->nBytesInBuffer == 0);
}

uint32 ECBDecipher::ReadBytes(void *destination, uint32 count)
{
	if(this->IsAtEnd())
		return 0;

	uint8* dest = static_cast<uint8 *>(destination);
	while(count)
	{
		//write decrypted bytes
		uint8 bytesToCopy = Math::Min(this->nBytesInBuffer, static_cast<uint8>(count));
		uint8 offset = static_cast<uint8>(this->decrypted.GetNumberOfElements() - this->nBytesInBuffer);
		MemCopy(dest, &this->decrypted[0]+offset, bytesToCopy);
		dest += bytesToCopy;
		count -= bytesToCopy;
		this->nBytesInBuffer -= bytesToCopy;

		//fill up buffer
		if(this->nBytesInBuffer == 0)
		{
			uint32 nBytesRead = this->inputStream.ReadBytes(&this->encrypted[0], this->encrypted.GetNumberOfElements());
			if(nBytesRead == 0)
				break;
			ASSERT(nBytesRead == this->encrypted.GetNumberOfElements(), u8"REPORT THIS PLEASE!");
			this->decipher->Decrypt(&this->encrypted[0], &this->decrypted[0]);
			if(this->inputStream.IsAtEnd())
			{
				//this is the last block, remove pkcs padding
				uint8 nPadding = this->decrypted.Last();

				//check padding
				for(uint8 i = static_cast<uint8>(this->decrypted.GetNumberOfElements() - nPadding); i < this->decrypted.GetNumberOfElements(); i++)
				{
					ASSERT(this->decrypted[i] == nPadding, u8"PKCS padding is corrupt!");
				}
				this->nBytesInBuffer = static_cast<uint8>(this->decrypted.GetNumberOfElements() - nPadding);

				//copy remaining bytes to end
				for(uint8 i = 0; i < this->nBytesInBuffer; i++)
					this->decrypted[this->decrypted.GetNumberOfElements() - this->nBytesInBuffer + i] = this->decrypted[i];
			}
			else
				this->nBytesInBuffer = static_cast<uint8>(this->decrypted.GetNumberOfElements());
		}
	}

	return static_cast<uint32>(dest - (uint8*)destination);
}

uint32 ECBDecipher::Skip(uint32 nBytes)
{
	NOT_IMPLEMENTED_ERROR;
	return 0;
}
