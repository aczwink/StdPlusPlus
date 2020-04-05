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
#include <Std++Test.hpp>
using namespace StdXX;

static void DateConstructorEquals(int64 year, uint8 month, uint8 day)
{
	Date unixEpoch(year, month, day);
	ASSERT_EQUALS(year, unixEpoch.Year());
	ASSERT_EQUALS(month, unixEpoch.Month());
	ASSERT_EQUALS(day, unixEpoch.Day());
}

static void TestLeapYears(uint32 year1, uint32 year2)
{
	int32 nLeapYears = 0;
	for(uint32 i = Math::Min(year1, year2); i <= Math::Max(year1, year2); i++)
	{
		if(WeakDate::IsLeapYear(i))
		{
			stdOut << i << endl;
			nLeapYears++;
		}
	}

	int64 first = Date::ComputeNumberOfLeapYears(year1, year2);
	ASSERT_EQUALS(Math::Abs(first), nLeapYears);

	int64 second = Date::ComputeNumberOfLeapYears(year2, year1);
	ASSERT_EQUALS(-first, second);
}

TEST_SUITE(DateTests)
{
	/*
	TEST_CASE(ComputationTest)
	{
		DateConstructorEquals(1502, 4, 25);
		DateConstructorEquals(1500, 4, 25);

		for(uint32 year = 1500; year <= 2500; year++)
		{
			for(uint8 month = 1; month <= 12; month++)
			{
				for(uint8 day = 1; day < WeakDate::GetNumberOfDaysInMonth(month, year); day++)
				{
					DateConstructorEquals(year, month, day);
				}
			}
		}
	}*/

	TEST_CASE(ComputeNumberOfLeapYearsTest)
	{
		TestLeapYears(1500, 1600);
		for(uint32 year1 = 1500; year1 <= 2500; year1++)
		{
			for(uint32 year2 = 1500; year2 <= 2500; year2++)
			{
				TestLeapYears(year1, year2);
			}
		}
	}

	TEST_CASE(ISOStringTest)
	{
		Date date(2020, 4, 3);

		const String expected = u8"2020-04-03";
		ASSERT_EQUALS(expected, date.ToISOString());

		Date parsed = Date::ParseISOString(expected);
		ASSERT_EQUALS(date, parsed);
	}
};