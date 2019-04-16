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

TEST_SUITE(PBKDF2)
{
	TEST(comparison_with_other_implementations)
	{
		const String password = u8"secret";
		const String salt = u8"salt";
		const uint32 nIterations = 100000;
		const String expected = u8"3fa094211c0cf2ed1d332ab43adc69aab469f0e0f2cae6345c81bb874eef3f9eb2c629052ec272ca49c2ee95b33e7ba6377b2317cd0dacce92c4748d3c7a45f0";

		const uint8* saltData = salt.ToUTF8().GetRawData();
		uint8 key[64];
		PBKDF2(password, saltData, static_cast<uint8>(salt.GetSize()), HashAlgorithm::SHA256, nIterations, key, sizeof(key));

		String resultStr;
		for(uint8 i : key)
			resultStr += String::HexNumber(i, 2, false).ToLowercase();
		ASSERT(resultStr == expected, u8"PBKDF2 mismatch!");
	}
};