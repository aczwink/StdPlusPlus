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

TEST_SUITE(CTR)
{
	TEST_CASE(enc_dec_test)
	{
		byte encrypted[16];
		const String key = u8"this is an insecure key!";
		const String data = u8"data";
		const uint8 nonce[12] = {};

		key.ToUTF8();
		data.ToUTF8();

		//encrypt
		BufferOutputStream encryptStream(encrypted, sizeof(encrypted));
		DefaultCounter<12, uint32> encryptCounter(nonce, 0);
		CTRCipher cipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize()), encryptCounter, encryptStream);

		cipher.WriteBytes(data.GetRawData(), data.GetSize());
		uint8 zero = 0;
		cipher.WriteBytes(&zero, 1);
		cipher.Flush();

		//decrypt
		BufferInputStream decryptStream(encrypted, sizeof(encrypted));
		DefaultCounter<12, uint32> decryptCounter(nonce, 0);
		CTRDecipher decipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize()), decryptCounter, decryptStream);
		TextReader textReader(decipher, TextCodecType::UTF8);

		String decrypted = textReader.ReadZeroTerminatedString();
		ASSERT(data == decrypted, u8"Expected data to not have changed by encryption and then decryption.");
	}

    TEST_CASE(enc_dec_test_with_nonce)
	{
		byte encrypted[89];
		const String key = u8"this is an insecure key!";
		const String data = u8"This is some random message that we encrypt here for demo purposes!!!!!!!!!!!!!!!!!!!!!!";
		const uint8 nonce[12] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
		const uint32 counter_initial = 4183462213;

		key.ToUTF8();
		data.ToUTF8();

		//encrypt
		BufferOutputStream encryptStream(encrypted, sizeof(encrypted));
		DefaultCounter<12, uint32> encryptCounter(nonce, counter_initial);
		CTRCipher cipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize()), encryptCounter, encryptStream);

		cipher.WriteBytes(data.GetRawData(), data.GetSize());
		uint8 zero = 0;
		cipher.WriteBytes(&zero, 1);
		cipher.Flush();

		//decrypt
		BufferInputStream decryptStream(encrypted, sizeof(encrypted));
		DefaultCounter<12, uint32> decryptCounter(nonce, counter_initial);
		CTRDecipher decipher(CipherAlgorithm::AES, key.GetRawData(), static_cast<uint16>(key.GetSize()), decryptCounter, decryptStream);
		TextReader textReader(decipher, TextCodecType::UTF8);

		String decrypted = textReader.ReadZeroTerminatedString();
		ASSERT(data == decrypted, u8"Expected data to not have changed by encryption and then decryption.");
	}
};