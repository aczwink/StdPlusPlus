/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
#include <StdXXTest.hpp>
using namespace StdXX;
using namespace StdXX::Crypto;

static void Execute(const Tuple<String, String>* testdata, uint32 nEntries, HashAlgorithm algorithm)
{
	for(uint32 i = 0; i < nEntries; i++)
	{
		const Tuple<String, String>& t = *testdata++;

		String input = t.Get<0>().ToUTF8();

		UniquePointer<Crypto::HashFunction> hasher = Crypto::HashFunction::CreateInstance(algorithm);
		hasher->Update(input.GetRawData(), input.GetSize());
		hasher->Finish();

		String expected = t.Get<1>();
		ASSERT(hasher->GetDigestString().ToLowercase() == expected, u8"Hash mismatch!");
	}
}

TEST_SUITE(SHATests)
{
    TEST_CASE(wikipedia_hashes_sha1)
	{
		//from https://en.wikipedia.org/wiki/SHA-1#Example_hashes
		const Tuple<String, String> testdata[] =
		{
			{
				u8"The quick brown fox jumps over the lazy dog",
				u8"2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"
			},
			{
				u8"The quick brown fox jumps over the lazy cog",
				u8"de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3"
			},
			{
				u8"",
				u8"da39a3ee5e6b4b0d3255bfef95601890afd80709"
			},
		};

		Execute(testdata, sizeof(testdata)/sizeof(testdata[0]), HashAlgorithm::SHA1);
	}

    TEST_CASE(wikipedia_hashes_sha256)
	{
		//from https://en.wikipedia.org/wiki/SHA-2#Test_vectors
		const Tuple<String, String> testdata[] =
		{
			{
				u8"",
				u8"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
			},
		};

		Execute(testdata, sizeof(testdata)/sizeof(testdata[0]), HashAlgorithm::SHA256);
	}

    TEST_CASE(wikipedia_hashes_sha512)
	{
		//from https://en.wikipedia.org/wiki/SHA-2#Test_vectors
		const Tuple<String, String> testdata[] =
		{
			{
				u8"",
				u8"cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e"
			},
		};

		Execute(testdata, sizeof(testdata)/sizeof(testdata[0]), HashAlgorithm::SHA512);
	}

	TEST_CASE(wikipedia_hashes_sha512_256)
	{
		//from https://en.wikipedia.org/wiki/SHA-2#Test_vectors
		const Tuple<String, String> testdata[] =
		{
			{
				u8"",
				u8"c672b8d1ef56ed28ab87c3622c5114069bdd3ad7b8f9737498d0c01ecef0967a"
			},
		};

		Execute(testdata, sizeof(testdata)/sizeof(testdata[0]), HashAlgorithm::SHA512_256);
	}
};