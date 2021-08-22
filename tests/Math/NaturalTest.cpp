/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
using namespace StdXX::Math;

TEST_SUITE(NaturalTest)
{
	TEST_CASE(addition1)
	{
		Natural e20(u8"100000000000000000000");
		ASSERT_EQUALS(Natural(u8"200000000000000000000"), e20 + e20);
	}

	TEST_CASE(addition2)
	{
		Natural n1(Unsigned<uint64>::Max());
		Natural n2(1);
		Natural sum = n1 + n2;

		ASSERT_EQUALS(Power(Natural(2), Natural(64)), sum);
	}

	TEST_CASE(comparion)
	{
		Natural n0;

		ASSERT_TRUE(n0 <= n0);
	}

	TEST_CASE(fromToString)
	{
		Natural n1000(u8"1000");
		ASSERT_EQUALS(u8"1000", n1000.ToString());
	}

	TEST_CASE(leftShift)
	{
		Natural n1 = 29461;
		Natural result = n1 << 0;
		ASSERT_EQUALS(n1, result);
	}

	TEST_CASE(multiplication)
	{
		Natural n1 = 29461;
		Natural n2 = 36089725;

		Natural result = n1 * n2;
		ASSERT_EQUALS(1063239388225, result.ClampTo64Bit());
	}

	TEST_CASE(division)
	{
		auto pow64 = Power(Natural(10), Natural(64));
		auto pow65 = pow64 * 10;
		auto pow65div10 = pow65 / 10;

		ASSERT_EQUALS(pow64, pow65div10);
	}
};