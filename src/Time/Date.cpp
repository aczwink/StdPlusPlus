/*
* Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/Time/Date.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;

//Local functions
static int64 YearMod(int64 year, int64 mod)
{
	return year - (year % mod);
}

//Constants
static const int c_epochYear = 1970;
const Date Date::Epoch(c_epochYear, 1, 1);

//Constructor
Date::Date(int64 year, uint8 month, uint8 day)
{
	ASSERT(WeakDate(year, month, day).IsValid(), u8"INVALID DATE");

	uint16 monthDays = 0;
	for (uint8 i = 1; i < month; i++)
		monthDays += WeakDate::GetNumberOfDaysInMonth(i, year);
	this->deltaDays = (year - c_epochYear) * 365
					  + Date::ComputeNumberOfLeapYears(c_epochYear, year) //exclude the current year, since its leap day is included in "monthDays"
			+ monthDays //includes the 29th February if "year" is a leap year and month > 2
			+ (day - 1); //day is one-based
}

//Properties
int64 Date::Year() const
{
	int64 curDays = this->deltaDays;

	int64 year = c_epochYear;
	do
	{
		const int64 relativeYear = curDays / ( (Math::Abs(curDays) > 365) ? 366 : 365);
		curDays -= relativeYear * 365;

		const int64 year_new = year + relativeYear;
		const int64 nLeapDays = Date::ComputeNumberOfLeapYears(year, year_new);
		curDays -= nLeapDays;

		year = year_new;
	}
	while( Math::Abs(curDays) >= 365 );

	if(curDays < 0)
		year--;

	return year;
}

//Class functions
int64 Date::ComputeNumberOfLeapYears(int64 fromYear, int64 toYear)
{
	if(fromYear == toYear)
		return 0;
	if(fromYear > toYear)
		return -ComputeNumberOfLeapYears(toYear, fromYear);

	toYear--; //exclude end
	int64 nLeapYears = (toYear - YearMod(fromYear, 4)) / 4;
	nLeapYears -= (toYear - YearMod(fromYear, 100)) / 100;
	nLeapYears += (toYear - YearMod(fromYear, 400)) / 400;

	if(WeakDate::IsLeapYear(fromYear))
		nLeapYears++;

	return nLeapYears;
}

Date Date::ParseISOString(const String& string)
{
	DynamicArray<String> parts = string.Split(u8"-");
	ASSERT_EQUALS(parts.GetNumberOfElements(), 3);

	return Date(parts[0].ToInt(), parts[1].ToUInt(), parts[2].ToUInt());
}

//Private methods
WeakDate Date::ToWeakDate() const
{
	int64 curDays = this->deltaDays;
	const int64 year = this->Year();
	const int64 relativeYear = year - 1970;

	curDays -= relativeYear * 365;
	const int64 nLeapDays = Date::ComputeNumberOfLeapYears(c_epochYear, year);
	curDays -= nLeapDays;
	curDays++; //days are one-based

	uint8 month;
	for (month = 1; curDays > WeakDate::GetNumberOfDaysInMonth(month, year); month++)
		curDays -= WeakDate::GetNumberOfDaysInMonth(month, year);

	return WeakDate(year, month, curDays);
}