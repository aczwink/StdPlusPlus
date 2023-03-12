/*
 * Copyright (c) 2023 Amir Czwink (amir130@hotmail.de)
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

TEST_SUITE(IntervalTest)
{
	TEST_CASE(adjacencyVsOverlap)
	{
		Interval r1(0, 1);
		Interval r2(1, 2);

		ASSERT(r1.Overlaps(r2), u8"1 is included in both ranges");

		Interval r3(2, 3);
		ASSERT(!r1.Overlaps(r3), u8"ranges are adjacent but not overlapping since the intersection is empty");
	}
};