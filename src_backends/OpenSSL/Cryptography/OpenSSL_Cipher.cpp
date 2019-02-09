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
#include "OpenSSL_Cipher.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
OpenSSL_Cipher::OpenSSL_Cipher(CipherAlgorithm algorithm, const byte* key, uint16 keyLength) : algorithm(algorithm)
{
	this->ctx = EVP_CIPHER_CTX_new();
	ASSERT(this->ctx, u8"Report this please!");

	int ret = EVP_EncryptInit_ex(this->ctx, this->MapCipherAlgorithm(algorithm, keyLength), nullptr, key, nullptr);
	ASSERT(ret == 1, u8"Report this please!");

	ret = EVP_CIPHER_CTX_set_padding(this->ctx, false);
	ASSERT(ret == 1, u8"Report this please!");
}

//Destructor
OpenSSL_Cipher::~OpenSSL_Cipher()
{
	EVP_CIPHER_CTX_free(this->ctx);
}

//Public methods
void OpenSSL_Cipher::Encrypt(const byte *unencrypted, byte *encrypted) const
{
	uint8 blockSize = this->GetBlockSize();
	int len;
	int ret = EVP_EncryptUpdate(this->ctx, encrypted, &len, unencrypted, blockSize);
	ASSERT(ret == 1, u8"Report this please!");
	ASSERT(len == blockSize, u8"Report this please!");

	ret = EVP_EncryptFinal_ex(this->ctx, encrypted + len, &len);
	ASSERT(ret == 1, u8"Report this please!");
	ASSERT(len == 0, u8"Report this please!");
}

uint8 OpenSSL_Cipher::GetBlockSize() const
{
	switch(algorithm)
	{
		case CipherAlgorithm::AES:
			return 16;
	}

	NOT_IMPLEMENTED_ERROR;
	return 0;
}

//Private methods
const EVP_CIPHER *OpenSSL_Cipher::MapCipherAlgorithm(CipherAlgorithm algorithm, uint16 keyLength) const
{
	switch(algorithm)
	{
		case CipherAlgorithm::AES:
		{
			switch(keyLength)
			{
				case 128:
					return EVP_aes_128_ecb();
				case 192:
					return EVP_aes_192_ecb();
				case 256:
					return EVP_aes_256_ecb();
				default:
					NOT_IMPLEMENTED_ERROR;
			}
		}
	}

	NOT_IMPLEMENTED_ERROR;
	return nullptr;
}