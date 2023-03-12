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

TEST_SUITE(IPRangeCombiningTests)
{
	TEST_CASE(combineRangesTest)
	{
		OrderedRangeSet<IPv4Address> rangeSet;

		rangeSet.Insert(IPv4Range(IPv4Address(u8"10.0.0.0"), IPv4Address(u8"10.0.0.100")));
		rangeSet.Insert(IPv4Range(IPv4Address(u8"10.0.0.101"), IPv4Address(u8"10.0.0.255")));

		ASSERT_EQUALS(1, rangeSet.Ranges().GetNumberOfElements());

		const auto& range = rangeSet.Ranges()[0];
		ASSERT_EQUALS(IPv4Address(u8"10.0.0.0"), range.start);
		ASSERT_EQUALS(IPv4Address(u8"10.0.0.255"), range.end);
	}

	TEST_CASE(rangeToCIDRTest)
	{
		IPv4Range range(IPv4Address(u8"10.0.0.0"), IPv4Address(u8"10.0.0.255"));

		auto result = range.ToMinimalCIDRRanges();
		ASSERT_EQUALS(1, result.GetNumberOfElements());

		auto cidr = result.Last();
		ASSERT_EQUALS(u8"10.0.0.0/24", cidr.ToString());
	}

	TEST_CASE(complexRangeToCIDRTest)
	{
		IPv4Range range(IPv4Address(u8"10.0.0.253"), IPv4Address(u8"10.0.0.255"));

		auto result = range.ToMinimalCIDRRanges();
		ASSERT_EQUALS(2, result.GetNumberOfElements());

		auto cidr1 = result[0];
		ASSERT_EQUALS(u8"10.0.0.254/31", cidr1.ToString());
		auto cidr2 = result[1];
		ASSERT_EQUALS(u8"10.0.0.253/32", cidr2.ToString());
	}
}