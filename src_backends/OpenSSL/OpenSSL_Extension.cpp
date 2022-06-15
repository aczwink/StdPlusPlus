/*
 * Copyright (c) 2019-2020,2022 Amir Czwink (amir130@hotmail.de)
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
#include "Cryptography/OpenSSL_Hasher.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Crypto;

//Public methods
void OpenSSL_Extension::Load()
{
	Extension::Load();

	ERR_load_CRYPTO_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(nullptr);
}

void OpenSSL_Extension::Unload()
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

UniquePointer<HashFunction> OpenSSL_Extension::CreateHasher(HashAlgorithm algorithm)
{
	const EVP_MD* messageDigest = OpenSSL_Extension::MapHashAlgorithm(algorithm);
	if(messageDigest)
		return new OpenSSL_Hasher(messageDigest);
	return nullptr;
}

//Internal class functions
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

const EVP_MD *OpenSSL_Extension::MapHashAlgorithm(HashAlgorithm algorithm)
{
	switch(algorithm)
	{
		case HashAlgorithm::MD5:
			return EVP_md5();
		case HashAlgorithm::SHA1:
			return EVP_sha1();
		case HashAlgorithm::SHA256:
			return EVP_sha256();
		case HashAlgorithm::SHA512:
			return EVP_sha512();
		case HashAlgorithm::SHA512_256:
			return EVP_sha512_256();
	}
	return nullptr;
}
