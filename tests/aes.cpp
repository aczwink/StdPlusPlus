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
#include <Std++Test.hpp>
using namespace StdXX;

TEST_SUITE(AESTest)
{
	TEST(encdec_tests)
	{
		//16 byte unencrypted block
		const String data = String(u8"test data string").ToUTF8();
		//32 byte (256 bit) key
		const String key = String(u8"this is an insecure key!!!!!!!!!").ToUTF8();

		//first encrypt
		UniquePointer<Cipher> cipher = Cipher::Create(CipherAlgorithm::AES, key.GetRawData(), key.GetSize() * 8);
		byte encrypted[16]; //block size of aes is 16 bytes
		cipher->Encrypt(data.GetRawData(), encrypted);

		//do it again
		byte encrypted2[16];
		cipher->Encrypt(data.GetRawData(), encrypted2);
		ASSERT(MemCmp(encrypted, encrypted2, sizeof(encrypted)) == 0, u8"Encrypting the same block twice should give the same result!");
	}
};