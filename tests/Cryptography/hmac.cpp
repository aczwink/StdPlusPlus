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

TEST_SUITE(HMAC)
{
	TEST(wikipedia_hashes)
	{
		//from https://en.wikipedia.org/wiki/HMAC#Examples
		const Tuple<HashAlgorithm, String, String, String> testdata[] = {
			{
				HashAlgorithm::MD5,
				u8"",
				u8"",
				u8"74e6f7298a9c2d168935f58c001bad88"
	 		},
			{
				HashAlgorithm::MD5,
				u8"key",
				u8"The quick brown fox jumps over the lazy dog",
				u8"80070713463e7749b90c2dc24911e275"
			},
			{
				HashAlgorithm::SHA1,
				u8"",
				u8"",
				u8"fbdb1d1b18aa6c08324b7d64b71fb76370690e1d"
			},
			{
				HashAlgorithm::SHA1,
				u8"key",
				u8"The quick brown fox jumps over the lazy dog",
				u8"de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9"
			},
			{
				HashAlgorithm::SHA256,
				u8"",
				u8"",
				u8"b613679a0814d9ec772f95d778c35fc5ff1697c493715653c6c712144292c5ad"
			},
			{
				HashAlgorithm::SHA256,
				u8"key",
				u8"The quick brown fox jumps over the lazy dog",
				u8"f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8"
			},
		};

		for(const auto& t : testdata)
		{
			HashAlgorithm hashAlgorithm = t.Get<0>();
			String key = t.Get<1>().ToUTF8();
			String msg = t.Get<2>().ToUTF8();
			String expected = t.Get<3>();

			UniquePointer<HashFunction> hasher = HashFunction::CreateInstance(hashAlgorithm);
			FixedArray<uint8> result(hasher->GetDigestSize());
			HMAC(key.GetRawData(), key.GetSize(), msg.GetRawData(), msg.GetSize(), hashAlgorithm, &result[0]);

			String resultStr;
			for(uint8 i : result)
				resultStr += String::HexNumber(i, 2, false).ToLowercase();
			ASSERT(resultStr == expected, u8"HMAC mismatch!");
		}
	}
};