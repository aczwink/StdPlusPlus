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

TEST_SUITE(ECB)
{
	TEST(padding) //block filled with padding
	{
		byte encrypted[16];
		const String key = u8"this is an insecure key!";
		const String data = u8"data";

		key.ToUTF8();
		data.ToUTF8();

		//encrypt
		BufferOutputStream encryptStream(encrypted, sizeof(encrypted));
		ECBCipher cipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize() * 8), encryptStream);

		cipher.WriteBytes(data.GetRawData(), data.GetSize());
		cipher.Flush();

		//decrypt
		BufferInputStream decryptStream(encrypted, sizeof(encrypted));
		ECBDecipher decipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize() * 8), decryptStream);
		TextReader textReader(decipher, TextCodecType::UTF8);

		String decrypted = textReader.ReadLine();
		ASSERT(data == decrypted, u8"Expected data to not have changed by encryption and then decryption.");
	}

	TEST(paddingExtraBlock) //one full extra padding block
	{
		byte encrypted[32];
		const String key = u8"this is an insecure key!";
		const String data = u8"data!padding!123";

		key.ToUTF8();
		data.ToUTF8();

		//encrypt
		BufferOutputStream encryptStream(encrypted, sizeof(encrypted));
		ECBCipher cipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize() * 8), encryptStream);

		cipher.WriteBytes(data.GetRawData(), data.GetSize());
		cipher.Flush();

		//decrypt
		BufferInputStream decryptStream(encrypted, sizeof(encrypted));
		ECBDecipher decipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize() * 8), decryptStream);
		TextReader textReader(decipher, TextCodecType::UTF8);

		String decrypted = textReader.ReadLine();
		ASSERT(data == decrypted, u8"Expected data to not have changed by encryption and then decryption.");
	}

	TEST(paddingTwoBlocks) //second block filled up with padding
	{
		byte encrypted[32];
		const String key = u8"this is an insecure key!";
		const String data = u8"data!padding!1234567890";

		key.ToUTF8();
		data.ToUTF8();

		//encrypt
		BufferOutputStream encryptStream(encrypted, sizeof(encrypted));
		ECBCipher cipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize() * 8), encryptStream);

		cipher.WriteBytes(data.GetRawData(), data.GetSize());
		cipher.Flush();

		//decrypt
		BufferInputStream decryptStream(encrypted, sizeof(encrypted));
		ECBDecipher decipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize() * 8), decryptStream);
		TextReader textReader(decipher, TextCodecType::UTF8);

		String decrypted = textReader.ReadLine();
		ASSERT(data == decrypted, u8"Expected data to not have changed by encryption and then decryption.");
	}

	TEST(paddingThreeBlocks) //third block filled up with padding
	{
		byte encrypted[48];
		const String key = u8"this is an insecure key!";
		const String data = u8"data!padding!1234567890!more padding!";

		key.ToUTF8();
		data.ToUTF8();

		//encrypt
		BufferOutputStream encryptStream(encrypted, sizeof(encrypted));
		ECBCipher cipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize() * 8), encryptStream);

		cipher.WriteBytes(data.GetRawData(), data.GetSize());
		cipher.Flush();

		//decrypt
		BufferInputStream decryptStream(encrypted, sizeof(encrypted));
		ECBDecipher decipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize() * 8), decryptStream);
		TextReader textReader(decipher, TextCodecType::UTF8);

		String decrypted = textReader.ReadLine();
		ASSERT(data == decrypted, u8"Expected data to not have changed by encryption and then decryption.");
	}
};