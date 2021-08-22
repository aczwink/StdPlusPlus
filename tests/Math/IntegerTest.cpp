/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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

TEST_SUITE(IntegerTest)
{
	TEST_CASE(addition1)
	{
		Integer m2(-2);
		Integer p3(3);
		ASSERT_EQUALS(1, (m2 + p3).ClampTo64Bit());

		Integer m10(-10);
		Integer p4(4);
		ASSERT_EQUALS(-6, (m10 + p4).ClampTo64Bit());
	}
};