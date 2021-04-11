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

TEST_SUITE(scrypt)
{
    TEST_CASE(comparison_with_other_implementations)
	{
		const String password = u8"secret";
		const String salt = u8"salt";
		constexpr uint8 keySize = 64;
		const String expected = u8"05ffaebcca41770af425d4ba9b4e7bcdff532237dca931c192a36d94db7307d4c2df95e606514b4113ccb3ad3c19f7ca648e373a112a6b8290f3a69818aa9b7e";

		const uint8* saltData = salt.ToUTF8().GetRawData();
		uint8 key[keySize];
		scrypt(password, saltData, static_cast<uint8>(salt.GetSize()), key, sizeof(key), 14);

		String resultStr;
		for(uint8 i : key)
			resultStr += String::HexNumber(i, 2, false).ToLowercase();
		resultStr.ToUTF8();
		ASSERT(resultStr == expected, u8"scrypt mismatch!");
	}
};