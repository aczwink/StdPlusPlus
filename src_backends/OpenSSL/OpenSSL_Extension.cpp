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
#include "OpenSSL_Extension.hpp"
//Global
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
//Local
#include "Cryptography/OpenSSL_BlockCipher.hpp"
#include "Cryptography/OpenSSL_BlockDecipher.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
void OpenSSL_Extension::Load()
{
	Extension::Load();

	ERR_load_CRYPTO_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(nullptr);
}

void OpenSSL_Extension::Unload() const
{
	Extension::Unload();

	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
	ERR_free_strings();
}

//Class functions
UniquePointer<BlockCipher> OpenSSL_Extension::CreateCipher(CipherAlgorithm algorithm, const byte* key, uint16 keyLength)
{
	uint8 blockSize;
	const EVP_CIPHER* cipher = OpenSSL_Extension::MapCipherAlgorithm(algorithm, keyLength, blockSize);
	if(cipher)
		return new OpenSSL_BlockCipher(cipher, key, blockSize);
	return nullptr;
}

UniquePointer<BlockDecipher> OpenSSL_Extension::CreateDecipher(CipherAlgorithm algorithm, const byte *key, uint16 keyLength)
{
	uint8 blockSize;
	const EVP_CIPHER* cipher = OpenSSL_Extension::MapCipherAlgorithm(algorithm, keyLength, blockSize);
	if(cipher)
		return new OpenSSL_BlockDecipher(cipher, key, blockSize);
	return nullptr;
}

//Private methods
const EVP_CIPHER* OpenSSL_Extension::MapCipherAlgorithm(StdXX::CipherAlgorithm algorithm, uint16 keyLength, uint8 &blockSize)
{
	switch(algorithm)
	{
		case CipherAlgorithm::AES:
		{
			blockSize = 16;
			switch(keyLength)
			{
				case 128:
					return EVP_aes_128_ecb();
				case 192:
					return EVP_aes_192_ecb();
				case 256:
					return EVP_aes_256_ecb();
			}
		}
	}

	return nullptr;
}