/*
 * Copyright (c) 2019,2021 Amir Czwink (amir130@hotmail.de)
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

class HMAC_HashFunction : public Crypto::HashFunction
{
public:
	//Constructor
	HMAC_HashFunction(const uint8 *key, uint8 keySize, Crypto::HashAlgorithm hashAlgorithm)
		: hashAlgorithm(hashAlgorithm)
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
			MemZero(&realKey[0] + keySize, blockSize - keySize); //pad with zeros

		//produce inner and outer key
		FixedArray<uint8> innerKey(blockSize);
		this->outerKey.Resize(blockSize);

		for(uint8 i = 0; i < blockSize; i++)
		{
			innerKey[i] = realKey[i] xor 0x36;
			outerKey[i] = realKey[i] xor 0x5C;
		}

		this->innerHasher = HashFunction::CreateInstance(hashAlgorithm);
		this->innerHasher->Update(&innerKey[0], blockSize);
	}

	uint32 GetBlockSize() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return 0;
	}

	uint32 GetDigestSize() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return 0;
	}

	void Finish() override
	{
		//hash inner
		FixedArray<uint8> tmp(this->innerHasher->GetDigestSize());

		this->innerHasher->Finish();
		this->innerHasher->StoreDigest(&tmp[0]);

		//hash outer
		this->outerHasher = HashFunction::CreateInstance(this->hashAlgorithm);
		this->outerHasher->Update(&outerKey[0], this->innerHasher->GetBlockSize());
		this->outerHasher->Update(&tmp[0], this->innerHasher->GetDigestSize());
		this->outerHasher->Finish();
	}

	void StoreDigest(void *target) const override
	{
		this->outerHasher->StoreDigest(target);
	}

	void Update(const void *buffer, uint32 size) override
	{
		this->innerHasher->Update(buffer, size);
	}

private:
	//Members
	Crypto::HashAlgorithm hashAlgorithm;
	UniquePointer<Crypto::HashFunction> innerHasher;
	DynamicArray<uint8> outerKey;
	UniquePointer<Crypto::HashFunction> outerHasher;
};

//Namespace functions
UniquePointer<Crypto::HashFunction> Crypto::CreateHMAC(const uint8 *key, uint8 keySize, Crypto::HashAlgorithm hashAlgorithm)
{
	return new HMAC_HashFunction(key, keySize, hashAlgorithm);
}

void Crypto::HMAC(const uint8 *key, uint8 keySize, const uint8 *msg, uint16 msgSize, HashAlgorithm hashAlgorithm, uint8* out)
{
	HMAC_HashFunction hashFunction(key, keySize, hashAlgorithm);

	hashFunction.Update(msg, msgSize);
	hashFunction.Finish();
	hashFunction.StoreDigest(out);
}