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

static void UnixTimeStampEquals(int64 unixTimeStamp, int64 year, uint8 month, uint8 day, uint8 hours, uint8 minutes, uint8 seconds)
{
	DateTime dt = DateTime::FromUnixTimeStamp(unixTimeStamp);
	ASSERT_EQUALS(dt.Date().Year(), year);
	ASSERT_EQUALS(dt.Date().Month(), month);
	ASSERT_EQUALS(dt.Date().Day(), day);

	ASSERT_EQUALS(dt.Time().Hours(), hours);
	ASSERT_EQUALS(dt.Time().Minutes(), minutes);
	ASSERT_EQUALS(dt.Time().Seconds(), seconds);
	ASSERT_EQUALS(dt.Time().MilliSeconds(), 0);
}

TEST_SUITE(DateTimeTests)
{
	TEST_CASE(ParseISOStringTest)
	{
		Date date(2020, 4, 3);
		Time time(16, 48, 22, 123);
		DateTime dateTime(date, time);

		const String expected = u8"2020-04-03 16:48:22.123";
		ASSERT_EQUALS(expected, dateTime.ToISOString());

		DateTime parsed = DateTime::ParseISOString(expected);
		ASSERT_EQUALS(dateTime, parsed);
	}

	TEST_CASE(UnixTimeStampTests)
	{
		UnixTimeStampEquals(Signed<int32>::Min(), 1901, 12, 13, 20, 45, 52);
	}
};