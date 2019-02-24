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
#include <Std++/Cryptography/CBCDecipher.hpp>
//Local
#include <Std++/Mathematics.hpp>
#include <Std++/Utility.hpp>
//Namespaces
using namespace StdXX::Crypto;

//Private methods
bool CBCDecipher::DecryptNextBlock()
{
	uint32 nBytesRead = this->inputStream.ReadBytes(this->currentBlock, this->encryptedBuf1.GetNumberOfElements());
	if(nBytesRead == 0)
		return false;
	ASSERT(nBytesRead == this->encryptedBuf1.GetNumberOfElements(), u8"REPORT THIS PLEASE!");
	this->decipher->Decrypt(this->currentBlock, this->GetDecryptAddress());

	uint8* addr = this->GetDecryptAddress();
	for(uint8 i = 0; i < this->encryptedBuf1.GetNumberOfElements(); i++)
		addr[i] = addr[i] xor this->lastBlock[i];
	Swap(this->currentBlock, this->lastBlock);

	return true;
}
