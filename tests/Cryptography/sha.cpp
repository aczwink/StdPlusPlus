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

TEST_SUITE(SHATests)
{
	TEST(wikipedia_hashes_sha256)
	{
		//from https://en.wikipedia.org/wiki/SHA-2#Test_vectors
		const Tuple<String, String> testdata[] =
		{
			{
				u8"",
				u8"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
			},
		};

		for(const Tuple<String, String>& t : testdata)
		{
			String input = t.Get<0>().ToUTF8();

			UniquePointer<HashFunction> hasher = HashFunction::CreateInstance(HashAlgorithm::SHA256);
			hasher->Update(input.GetRawData(), input.GetSize());
			hasher->Finish();

			String expected = t.Get<1>();
			ASSERT(hasher->GetDigestString().ToLowercase() == expected, u8"SHA256 mismatch!");
		}
	}
};