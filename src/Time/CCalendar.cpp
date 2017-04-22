/*
* Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
*
* This file is part of ACStdLib.
*
* ACStdLib is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ACStdLib is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
*/
//Class Header
#include <ACStdLib/Time/CCalendar.h>
//Namespaces
using namespace ACStdLib;

//Methods
uint8 CCalendar::GetNumberOfDaysInMonth(uint8 month, uint32 year)
{
	switch(month)
	{
	case 2:
		return 28 + this->IsLeapYear(year);
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	}

	return 31;
}

bool CCalendar::IsLeapYear(uint32 year)
{
	if((year % 100 == 0) && (year % 400 != 0))
		return false;

	return (year % 4 == 0);
}