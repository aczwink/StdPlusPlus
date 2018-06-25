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
//Namespaces
using namespace StdPlusPlus;

//Class functions
uint8 Date::GetNumberOfDaysInMonth(uint8 month, int64 year)
{
	switch (month)
	{
	case 2:
		return 28 + Date::IsLeapYear(year);
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	}

	return 31;
}

bool Date::IsLeapYear(int64 year)
{
	if ((year % 100 == 0) && (year % 400 != 0))
		return false;

	return (year % 4 == 0);
}