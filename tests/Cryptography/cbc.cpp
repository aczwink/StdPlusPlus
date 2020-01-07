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
using namespace StdXX::Crypto;

TEST_SUITE(CBC)
{
    TEST_CASE(padding) //block filled with padding
	{
		byte encrypted[16];
		const String key = u8"this is an insecure key!";
		const String data = u8"data";
		const uint8 iv[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

		key.ToUTF8();
		data.ToUTF8();

		//encrypt
		BufferOutputStream encryptStream(encrypted, sizeof(encrypted));
		CBCCipher cipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize()), iv, encryptStream);

		cipher.WriteBytes(data.GetRawData(), data.GetSize());
		cipher.Finalize();

		//decrypt
		BufferInputStream decryptStream(encrypted, sizeof(encrypted));
		CBCDecipher decipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize()), iv, decryptStream);
		TextReader textReader(decipher, TextCodecType::UTF8);

		String decrypted = textReader.ReadLine();
		ASSERT(data == decrypted, u8"Expected data to not have changed by encryption and then decryption.");
	}

    TEST_CASE(paddingExtraBlock) //one full extra padding block
	{
		byte encrypted[32];
		const String key = u8"this is an insecure key!";
		const String data = u8"data!padding!123";
		const uint8 iv[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

		key.ToUTF8();
		data.ToUTF8();

		//encrypt
		BufferOutputStream encryptStream(encrypted, sizeof(encrypted));
		CBCCipher cipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize()), iv, encryptStream);

		cipher.WriteBytes(data.GetRawData(), data.GetSize());
		cipher.Finalize();

		//decrypt
		BufferInputStream decryptStream(encrypted, sizeof(encrypted));
		CBCDecipher decipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize()), iv, decryptStream);
		TextReader textReader(decipher, TextCodecType::UTF8);

		String decrypted = textReader.ReadLine();
		ASSERT(data == decrypted, u8"Expected data to not have changed by encryption and then decryption.");
	}

    TEST_CASE(paddingTwoBlocks) //second block filled up with padding
	{
		byte encrypted[32];
		const String key = u8"this is an insecure key!";
		const String data = u8"data!padding!1234567890";
		const uint8 iv[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

		key.ToUTF8();
		data.ToUTF8();

		//encrypt
		BufferOutputStream encryptStream(encrypted, sizeof(encrypted));
		CBCCipher cipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize()), iv, encryptStream);

		cipher.WriteBytes(data.GetRawData(), data.GetSize());
		cipher.Finalize();

		//decrypt
		BufferInputStream decryptStream(encrypted, sizeof(encrypted));
		CBCDecipher decipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize()), iv, decryptStream);
		TextReader textReader(decipher, TextCodecType::UTF8);

		String decrypted = textReader.ReadLine();
		ASSERT(data == decrypted, u8"Expected data to not have changed by encryption and then decryption.");
	}

    TEST_CASE(paddingThreeBlocks) //third block filled up with padding
	{
		byte encrypted[48];
		const String key = u8"this is an insecure key!";
		const String data = u8"data!padding!1234567890!more padding!";
		const uint8 iv[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

		key.ToUTF8();
		data.ToUTF8();

		//encrypt
		BufferOutputStream encryptStream(encrypted, sizeof(encrypted));
		CBCCipher cipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize()), iv, encryptStream);

		cipher.WriteBytes(data.GetRawData(), data.GetSize());
		cipher.Finalize();

		//decrypt
		BufferInputStream decryptStream(encrypted, sizeof(encrypted));
		CBCDecipher decipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize()), iv, decryptStream);
		TextReader textReader(decipher, TextCodecType::UTF8);

		String decrypted = textReader.ReadLine();
		ASSERT(data == decrypted, u8"Expected data to not have changed by encryption and then decryption.");
	}
};