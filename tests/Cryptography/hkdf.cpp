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

TEST_SUITE(HKDF)
{
	uint8 FromHexDigit(uint32 codePoint)
	{
		if(Math::IsValueInInterval(char(codePoint), u8'0', u8'9'))
			return codePoint - u8'0';
		return codePoint + 10 - u8'a';
	}

	FixedArray<uint8> HexStringToBytes(const String& string)
	{
		FixedArray<uint8> buffer(string.GetLength()/2);

		auto it = string.begin();
		for(uint8 i = 0; i < buffer.GetNumberOfElements(); i++)
		{
			buffer[i] = FromHexDigit(*it) << 4;
			++it;
			buffer[i] |= FromHexDigit(*it);
			++it;
		}
		return buffer;
	}

    TEST_CASE(official_test_vectors) //from https://tools.ietf.org/html/rfc5869
	{
		/*
		 * hash algorithm
		 * ikm
		 * salt
		 * info
		 * L
		 * PRK
		 * OKM
		 */
		const Tuple<HashAlgorithm, String, String, String, uint8, String> testData[] = {
			{ //test case 1
				HashAlgorithm::SHA256,
				u8"0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b",
				u8"000102030405060708090a0b0c",
				u8"f0f1f2f3f4f5f6f7f8f9",
				42,
				u8"3cb25f25faacd57a90434f64d0362f2a2d2d0a90cf1a5a4c5db02d56ecc4c5bf34007208d5b887185865"
			},
			{ //test case 2
				HashAlgorithm::SHA256,
				u8"000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f",
				u8"606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeaf",
				u8"b0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff",
				82,
				u8"b11e398dc80327a1c8e7f78c596a49344f012eda2d4efad8a050cc4c19afa97c59045a99cac7827271cb41c65e590e09da3275600c2f09b8367793a9aca3db71cc30c58179ec3e87c14c01d5c1f3434f1d87"
			},
			{ //test case 4
				HashAlgorithm::SHA1,
				u8"0b0b0b0b0b0b0b0b0b0b0b",
				u8"000102030405060708090a0b0c",
				u8"f0f1f2f3f4f5f6f7f8f9",
				42,
				u8"085a01ea1b10f36933068b56efa5ad81a4f14b822f5b091568a9cdd4f155fda2c22e422478d305f3f896"
			},
			{ //test case 5
				HashAlgorithm::SHA1,
				u8"000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f",
				u8"606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeaf",
				u8"b0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff",
				82,
				u8"0bd770a74d1160f7c9f12cd5912a06ebff6adcae899d92191fe4305673ba2ffe8fa3f1a4e5ad79f3f334b3b202b2173c486ea37ce3d397ed034c7f9dfeb15c5e927336d0441f4c4300e2cff0d0900b52d3b4"
			},
		};

		for(const auto& testCaseData : testData)
		{
			auto ikm = HexStringToBytes(testCaseData.Get<1>());
			auto salt = HexStringToBytes(testCaseData.Get<2>());
			auto info = HexStringToBytes(testCaseData.Get<3>());
			auto expectedKey = HexStringToBytes(testCaseData.Get<5>());

			FixedArray<uint8> key(testCaseData.Get<4>());
			HKDF(&ikm[0], ikm.GetNumberOfElements(), &salt[0], salt.GetNumberOfElements(), &info[0],
				 info.GetNumberOfElements(), testCaseData.Get<0>(), &key[0], key.GetNumberOfElements());

			int32 cmp = MemCmp(&key[0], &expectedKey[0], testCaseData.Get<4>());
			ASSERT(cmp == 0, u8"HKDF mismatch!");
		}
	}

    TEST_CASE(official_test_vectors_without_salt_and_info) //from https://tools.ietf.org/html/rfc5869
	{
		/*
		 * hash algorithm
		 * ikm
		 * L
		 * PRK
		 * OKM
		 */
		const Tuple<HashAlgorithm, String, uint8, String> testData[] = {
			{ //test case 3
				HashAlgorithm::SHA256,
				u8"0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b",
				42,
				u8"8da4e775a563c18f715f802a063c5a31b8a11f5c5ee1879ec3454e5f3c738d2d9d201395faa4b61a96c8"
			},
			{ //test case 6
				HashAlgorithm::SHA1,
				u8"0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b",
				42,
				u8"0ac1af7002b3d761d1e55298da9d0506b9ae52057220a306e07b6b87e8df21d0ea00033de03984d34918"
			},
			{ //test case 7
				HashAlgorithm::SHA1,
				u8"0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c",
				42,
				u8"2c91117204d745f3500d636a62f64f0ab3bae548aa53d423b0d1f27ebba6f5e5673a081d70cce7acfc48"
			},
		};

		for(const auto& testCaseData : testData)
		{
			auto ikm = HexStringToBytes(testCaseData.Get<1>());
			auto expectedKey = HexStringToBytes(testCaseData.Get<3>());

			FixedArray<uint8> key(testCaseData.Get<2>());
			HKDF(&ikm[0], ikm.GetNumberOfElements(), nullptr, 0, testCaseData.Get<0>(), &key[0], key.GetNumberOfElements());

			int32 cmp = MemCmp(&key[0], &expectedKey[0], testCaseData.Get<2>());
			ASSERT(cmp == 0, u8"HKDF mismatch!");
		}
	}
};