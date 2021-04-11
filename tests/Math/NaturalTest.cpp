/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
	TEST_CASE(multiplication)
	{
		Natural n1 = 29461;
		Natural n2 = 36089725;

		Natural result = n1 * n2;
		ASSERT_EQUALS(1063239388225, result.RoundDown());
	}

	TEST_CASE(leftShift)
	{
		Natural n1 = 29461;
		Natural result = n1 << 0;
		ASSERT_EQUALS(n1, result);
	}
};