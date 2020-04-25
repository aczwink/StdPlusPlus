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
	ASSERT_EQUALS(dt.Time().Milliseconds(), 0);
}

TEST_SUITE(DateTimeTests)
{
	TEST_CASE(ParseISOFractionalSecondsTest)
	{
		const String less = u8"2020-04-03T16:48:22.123Z";
		DateTime parsed = DateTime::ParseISOString(less);
		ASSERT_EQUALS(123 * 1000 * 1000, parsed.Time().NanosecondsSinceStartOfSecond());

		const String more = u8"2020-04-03T16:48:22.1234567899Z";
		DateTime parsed2 = DateTime::ParseISOString(more);
		ASSERT_EQUALS(123456789, parsed2.Time().NanosecondsSinceStartOfSecond());
	}

	TEST_CASE(ParseISOStringTest)
	{
		Date date(2020, 4, 3);
		Time time(16, 48, 22, 123, 456, 789);
		DateTime dateTime(date, time);

		const String expected = u8"2020-04-03T16:48:22.123456789Z";
		ASSERT_EQUALS(expected, dateTime.ToISOString());

		DateTime parsed = DateTime::ParseISOString(expected);
		ASSERT_EQUALS(dateTime, parsed);
	}

	TEST_CASE(UnixTimeStampTests)
	{
		UnixTimeStampEquals(Signed<int32>::Min(), 1901, 12, 13, 20, 45, 52);
		UnixTimeStampEquals(-1000000000, 1938, 4, 24, 22, 13, 20);
		UnixTimeStampEquals(0, 1970, 1, 1, 0, 0, 0);
		UnixTimeStampEquals(100000000, 1973, 3, 3, 9, 46, 40);
		UnixTimeStampEquals(500000000, 1985, 11, 5, 0, 53, 20);
		UnixTimeStampEquals(1000000000, 2001, 9, 9, 1, 46, 40);
		UnixTimeStampEquals(1111111111, 2005, 3, 18, 1, 58, 31);
		UnixTimeStampEquals(1234567890, 2009, 2, 13, 23, 31, 30);
		UnixTimeStampEquals(2000000000, 2033, 5, 18, 3, 33, 20);
		UnixTimeStampEquals(Signed<int32>::Max(), 2038, 1, 19, 3, 14, 7);
		UnixTimeStampEquals(Unsigned<uint32>::Max(), 2106, 2, 7, 6, 28, 15);
		UnixTimeStampEquals(Signed<int64>::Max(), 292277026596, 12, 4, 15, 30, 7);
	}
};