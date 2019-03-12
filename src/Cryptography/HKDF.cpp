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
//Corresponding header
#include <Std++/Cryptography/KeyDerivationFunctions.hpp>
//Local
#include <Std++/Cryptography/HMAC.hpp>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Crypto;

//Local functions
static void HKDF_Expand(const uint8* prk, uint8 prkSize, const uint8* info, uint8 infoSize, HashAlgorithm hashAlgorithm, uint8 hashLen, uint8* key, uint8 keySize)
{
	const uint8 bufferSize = hashLen + infoSize + 1;
	FixedArray<uint8> firstBuffer(bufferSize), secondBuffer(bufferSize);

	uint8* currentBlock = &firstBuffer[0];
	uint8* lastBlock = &secondBuffer[0];

	//generate first block
	MemCopy(currentBlock, info, infoSize);
	currentBlock[infoSize] = 1;
	HMAC(prk, prkSize, currentBlock, static_cast<uint16>(infoSize + 1), hashAlgorithm, lastBlock);

	uint8 bytesToCopy = Math::Min(keySize, hashLen);
	MemCopy(key, lastBlock, bytesToCopy);
	key += bytesToCopy;
	keySize -= bytesToCopy;

	//remaining blocks
	uint8 counter = 2;
	while(keySize)
	{
		MemCopy(&lastBlock[hashLen], info, infoSize);
		lastBlock[hashLen + infoSize] = counter;

		HMAC(prk, prkSize, lastBlock, bufferSize, hashAlgorithm, currentBlock);

		bytesToCopy = Math::Min(keySize, hashLen);
		MemCopy(key, currentBlock, bytesToCopy);
		key += bytesToCopy;
		keySize -= bytesToCopy;

		Swap(currentBlock, lastBlock);
		counter++;
	}
}

//Namespace functions
void StdXX::Crypto::HKDF(const uint8 *ikm, uint8 ikmSize, const uint8* info, uint8 infoSize, HashAlgorithm hashAlgorithm, uint8* key, uint8 keySize)
{
	UniquePointer<HashFunction> hasher = HashFunction::CreateInstance(hashAlgorithm);

	//generate salt
	FixedArray<uint8> salt(hasher->GetDigestSize());
	MemZero(&salt[0], salt.GetNumberOfElements());

	//extract
	FixedArray<uint8> prk(hasher->GetDigestSize());
	HMAC(&salt[0], static_cast<uint8>(salt.GetNumberOfElements()), ikm, ikmSize, hashAlgorithm, &prk[0]);

	//expand
	HKDF_Expand(&prk[0], prk.GetNumberOfElements(), info, infoSize, hashAlgorithm, hasher->GetDigestSize(), key, keySize);
}

void StdXX::Crypto::HKDF(const uint8 *ikm, uint8 ikmSize, const uint8 *salt, uint8 saltSize, const uint8* info, uint8 infoSize, HashAlgorithm hashAlgorithm, uint8* key, uint8 keySize)
{
	UniquePointer<HashFunction> hasher = HashFunction::CreateInstance(hashAlgorithm);

	//extract
	FixedArray<uint8> prk(hasher->GetDigestSize());
	HMAC(salt, saltSize, ikm, ikmSize, hashAlgorithm, &prk[0]);

	//expand
	HKDF_Expand(&prk[0], prk.GetNumberOfElements(), info, infoSize, hashAlgorithm, hasher->GetDigestSize(), key, keySize);
}