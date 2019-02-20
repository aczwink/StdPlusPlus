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
#include <Std++/Streams/Writers/DataWriter.hpp>
#include <Std++/Streams/BufferOutputStream.hpp>
#include <Std++/Cryptography/HMAC.hpp>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;

void StdXX::Crypto::PBKDF2(const String &password, const uint8 *salt, uint8 saltSize, HashAlgorithm hashAlgorithm, uint32 nIterations, uint8 *key, uint8 keySize)
{
	UniquePointer<HashFunction> hasher = HashFunction::CreateInstance(hashAlgorithm);
	const uint8 digestSize = static_cast<const uint8>(hasher->GetDigestSize());
	const uint32 blockCount = Unsigned<uint32>::DivCeil(keySize, digestSize);

	byte buf1[digestSize], buf2[digestSize], xored[digestSize];
	for(uint32 i = 0; i < blockCount; i++)
	{
		//first iteration
		const uint8 u0Size = saltSize + 4;
		byte u0[u0Size];
		MemCopy(u0, salt, saltSize);

		BufferOutputStream outputStream(u0 + saltSize, 4);
		DataWriter dataWriter(true, outputStream);

		dataWriter.WriteUInt32(i+1);

		uint8* last = buf1;
		HMAC(password.GetRawData(), password.GetSize(), u0, u0Size, hashAlgorithm, last);
		MemCopy(xored, last, digestSize);

		//rest iterations
		uint8* next = buf2;
		for(uint32 j = 1; j < nIterations; j++)
		{
			HMAC(password.GetRawData(), password.GetSize(), last, digestSize, hashAlgorithm, next);
			for(uint8 k = 0; k < digestSize; k++)
				xored[k] = xored[k] xor next[k];
			Swap(last, next);
		}

		//write to output
		uint8 validBytes = Math::Min(digestSize, keySize);
		MemCopy(key, xored, validBytes);
		key += validBytes;
		keySize -= validBytes;
	}
}
