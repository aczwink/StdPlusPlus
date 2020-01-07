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

TEST_SUITE(PathTest)
{
	const String expected = u8"/an/example/path";

    TEST_CASE(Normalized_AlreadyNormalized)
	{
		Path p = String(u8"/an/example/path");

		ASSERT(p.Normalized() == expected, u8"An already normalized path must not be altered.");
	}

    TEST_CASE(Normalized_DoubleSlash)
	{
		Path p = String(u8"/an/example//path");

		ASSERT(p.Normalized() == expected, u8"Normalized path is wrong.");
	}

    TEST_CASE(Normalized_SkipSingleDot)
	{
		Path p = String(u8"/an/example/./path");

		ASSERT(p.Normalized() == expected, u8"Normalized path is wrong.");
	}

    TEST_CASE(Normalized_ResolveDoubleDots)
	{
		Path p = String(u8"/an/example/another/../path");

		ASSERT(p.Normalized() == expected, u8"Normalized path is wrong.");
	}
};