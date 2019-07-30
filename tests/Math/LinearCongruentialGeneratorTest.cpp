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
using namespace StdXX::Math;

TEST_SUITE(LinearCongruentialGeneratorTest)
{
	TEST(comparison_with_cpp11_std)
	{
		MinStdRand rbg;

		ASSERT(rbg.Next() == 48271, u8"Wrong random number.");
		ASSERT(rbg.Next() == 182605794, u8"Wrong random number.");
		ASSERT(rbg.Next() == 1291394886, u8"Wrong random number.");
		ASSERT(rbg.Next() == 1914720637, u8"Wrong random number.");
		ASSERT(rbg.Next() == 2078669041, u8"Wrong random number.");
		ASSERT(rbg.Next() == 407355683, u8"Wrong random number.");
	}
};