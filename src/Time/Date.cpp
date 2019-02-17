/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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

//Constructor
Date::Date(int64 year, uint8 month, uint8 day)
{
	ASSERT(WeakDate(year, month, day).IsValid(), u8"INVALID DATE");

	uint16 monthDays = 0;
	for (uint8 i = 1; i < month; i++)
		monthDays += WeakDate::GetNumberOfDaysInMonth(i, year);
	this->deltaDays = (year - 1970) * 365 + Date::GetNumberOfElapsedLeapYears(year - 1970) + monthDays + (day-1); //day is one-based
}

//Class functions
uint64 Date::GetNumberOfElapsedLeapYears(uint64 year)
{
	uint64 nLeapYears = year / 4;
	nLeapYears -= year / 100;
	nLeapYears += (year + 300) / 400;

	return nLeapYears;
}

//Private methods
WeakDate Date::ToWeakDate() const
{
	int64 curDays = this->deltaDays;
	int64 relativeYear = curDays / 365;
	curDays -= relativeYear * 365;
	curDays -= Date::GetNumberOfElapsedLeapYears(relativeYear);

	uint8 month;
	for (month = 1; curDays > WeakDate::GetNumberOfDaysInMonth(month, relativeYear); month++)
		curDays -= WeakDate::GetNumberOfDaysInMonth(month, relativeYear);

	return WeakDate(relativeYear + 1970, month, curDays + 1);
}