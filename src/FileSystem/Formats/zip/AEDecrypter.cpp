/*
* Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include "AEDecrypter.hpp"
//Local
#include <Std++/Cryptography/KeyDerivationFunctions.hpp>
#include <Std++/Mathematics.hpp>
#include <Std++/Cryptography/HMAC.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
AEDecrypter::AEDecrypter(uint32 compressedSize, uint8 strength, const String& password, InputStream& inputStream, bool verify)
	: baseInputStream(inputStream)
{
	const uint8 keySizes[] = { 16, 24, 32 };
	const uint8 saltSizes[] = { 8, 12, 16 };

	uint8 saltSize = saltSizes[strength-1];
	byte salt[16];

	uint32 nBytesRead = inputStream.ReadBytes(salt, saltSize);
	ASSERT_EQUALS(saltSize, nBytesRead);

	byte verification[2];
	nBytesRead = inputStream.ReadBytes(verification, sizeof(verification));
	ASSERT_EQUALS(sizeof(verification), nBytesRead);

	uint8 singleKeySize = keySizes[strength-1];
	uint8 keyLength = 2 * singleKeySize + sizeof(verification);
	byte key[32*2+2];
	Crypto::PBKDF2(password, salt, saltSize, Crypto::HashAlgorithm::SHA1, 1000, key, keyLength);

	ASSERT_EQUALS(key[2 * singleKeySize], verification[0]);
	ASSERT_EQUALS(key[2 * singleKeySize + 1], verification[1]);

	InputStream* decipherBaseStream = &inputStream;
	if(verify)
	{
		this->authenticator = Crypto::CreateHMAC(&key[singleKeySize], singleKeySize, Crypto::HashAlgorithm::SHA1);
		this->authenticatorStream = new Crypto::HashingInputStream(inputStream, this->authenticator.operator->());
		decipherBaseStream = this->authenticatorStream.operator->();
	}

	this->decipher = new Crypto::CTRDecipher(CipherAlgorithm::AES, key, singleKeySize, this->counter, *decipherBaseStream);

	this->leftCompressedSize = compressedSize - saltSize - sizeof(verification) - 10;
}

//Public methods
uint32 _stdxx_::AEDecrypter::GetBytesAvailable() const
{
	NOT_IMPLEMENTED_ERROR;
	return 0;
}

bool _stdxx_::AEDecrypter::IsAtEnd() const
{
	NOT_IMPLEMENTED_ERROR;
	return false;
}

uint32 AEDecrypter::ReadBytes(void *destination, uint32 count)
{
	if(this->leftCompressedSize == 0)
		return 0;

	uint8* dest = static_cast<uint8 *>(destination);

	while(count)
	{
		if(this->leftCompressedSize == 0)
		{
			if(!this->authenticator.IsNull())
			{
				byte auth[10];
				uint32 nBytesRead = this->baseInputStream.ReadBytes(auth, sizeof(auth));
				ASSERT_EQUALS(sizeof(auth), nBytesRead);

				this->authenticator->Finish();
				byte computed[20];
				this->authenticator->StoreDigest(computed);

				ASSERT_EQUALS(0, MemCmp(auth, computed, sizeof(auth)));
			}
			break;
		}

		uint32 nBytesToRead = Math::Min(this->leftCompressedSize, count);
		uint32 nBytesRead = this->decipher->ReadBytes(dest, nBytesToRead);

		dest += nBytesRead;
		count -= nBytesRead;
		this->leftCompressedSize -= nBytesRead;
	}

	return dest - static_cast<uint8 *>(destination);
}

uint32 _stdxx_::AEDecrypter::Skip(uint32 nBytes)
{
	NOT_IMPLEMENTED_ERROR;
	return 0;
}