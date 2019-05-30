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
#include <Std++/Cryptography/HMAC.hpp>
//Local
#include <Std++/Memory.hpp>
//Namespaces
using namespace StdXX;

void StdXX::Crypto::HMAC(const uint8 *key, uint8 keySize, const uint8 *msg, uint16 msgSize, HashAlgorithm hashAlgorithm, uint8* out)
{
	UniquePointer<HashFunction> hasher = HashFunction::CreateInstance(hashAlgorithm);
	const uint8 blockSize = static_cast<const uint8>(hasher->GetBlockSize());
	const uint8 digestSize = static_cast<const uint8>(hasher->GetDigestSize());

	FixedArray<uint8> realKey(blockSize);
	if(keySize > blockSize)
	{
		hasher->Update(key, keySize);
		hasher->Finish();
		hasher->StoreDigest(&realKey[0]);
		keySize = digestSize;
	}
	else
	{
		MemCopy(&realKey[0], key, keySize);
	}

	if(keySize < blockSize)
		MemZero(&realKey[keySize], blockSize - keySize); //pad with zeros

	//produce inner and outer key
	FixedArray<uint8> innerKey(blockSize);
	FixedArray<uint8> outerKey(blockSize);

	for(uint8 i = 0; i < blockSize; i++)
	{
		innerKey[i] = realKey[i] xor 0x36;
		outerKey[i] = realKey[i] xor 0x5C;
	}

	//hash inner
	FixedArray<uint8> tmp(digestSize);

	hasher = HashFunction::CreateInstance(hashAlgorithm);
	hasher->Update(&innerKey[0], blockSize);
	hasher->Update(msg, msgSize);
	hasher->Finish();
	hasher->StoreDigest(&tmp[0]);

	//hash outer
	hasher = HashFunction::CreateInstance(hashAlgorithm);
	hasher->Update(&outerKey[0], blockSize);
	hasher->Update(&tmp[0], digestSize);
	hasher->Finish();
	hasher->StoreDigest(out);
}
