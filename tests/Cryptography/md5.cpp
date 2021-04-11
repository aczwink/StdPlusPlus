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
#include <StdXXTest.hpp>
using namespace StdXX;
using namespace StdXX::Crypto;

TEST_SUITE(MD5Tests)
{
    TEST_CASE(wikipedia_hashes)
	{
		//from https://en.wikipedia.org/wiki/MD5#MD5_hashes
		const Tuple<String, String> testdata[] = {
			{
				u8"The quick brown fox jumps over the lazy dog",
				u8"9e107d9d372bb6826bd81d3542a419d6"
			},
			{
				u8"The quick brown fox jumps over the lazy dog.",
				u8"e4d909c290d0fb1ca068ffaddf22cbd0"
			},
			{
				u8"",
				u8"d41d8cd98f00b204e9800998ecf8427e"
			},
		};

		for(const Tuple<String, String>& t : testdata)
		{
			String input = t.Get<0>().ToUTF8();

			UniquePointer<Crypto::HashFunction> hasher = Crypto::HashFunction::CreateInstance(HashAlgorithm::MD5);
			hasher->Update(input.GetRawData(), input.GetSize());
			hasher->Finish();

			String expected = t.Get<1>();
			ASSERT(hasher->GetDigestString().ToLowercase() == expected, u8"MD5 mismatch!");
		}
	}
};