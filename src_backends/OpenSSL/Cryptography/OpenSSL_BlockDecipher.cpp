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
#include "OpenSSL_BlockDecipher.hpp"
//Namespaces
using namespace _stdxx_;

//Constructor
OpenSSL_BlockDecipher::OpenSSL_BlockDecipher(const EVP_CIPHER* cipher, const byte* key, uint8 blockSize) : blockSize(blockSize)
{
	this->ctx = EVP_CIPHER_CTX_new();
	ASSERT(this->ctx, u8"Report this please!");

	int ret = EVP_DecryptInit_ex(this->ctx, cipher, nullptr, key, nullptr);
	ASSERT(ret == 1, u8"Report this please!");

	ret = EVP_CIPHER_CTX_set_padding(this->ctx, false);
	ASSERT(ret == 1, u8"Report this please!");
}

//Destructor
OpenSSL_BlockDecipher::~OpenSSL_BlockDecipher()
{
	EVP_CIPHER_CTX_free(this->ctx);
}

//Public methods
void OpenSSL_BlockDecipher::Decrypt(const void* encrypted, byte *unencrypted) const
{
	uint8 blockSize = this->GetBlockSize();
	int len;
	int ret = EVP_DecryptUpdate(this->ctx, unencrypted, &len, (const unsigned char*)encrypted, blockSize);
	ASSERT(ret == 1, u8"Report this please!");
	ASSERT(len == blockSize, u8"Report this please!");

	ret = EVP_DecryptFinal_ex(this->ctx, unencrypted + len, &len);
	ASSERT(ret == 1, u8"Report this please!");
	ASSERT(len == 0, u8"Report this please!");
}

uint8 OpenSSL_BlockDecipher::GetBlockSize() const
{
	return this->blockSize;
}
