/*
* Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
//Class Header
#include <Std++/Time/DateTime.hpp>
//Local
#include <Std++/Debug.hpp>
//Namespaces
using namespace StdPlusPlus;

//Class functions
DateTime DateTime::FromUnixTimeStampWithMilliSeconds(int64 timeStamp)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	//return DateTime();
}

/*
//Local functions
static uint32 GetNumberOfElapsedLeapYears(uint32 year)
{
	int32 nLeapYears;

	nLeapYears = year / 4;
	nLeapYears -= year / 100;
	nLeapYears += (year + 300) / 400;

	return nLeapYears;
}

//Private methods
void Time::CalcDateTimeInfo(SDateTimeInfo &refDateTimeInfo) const
{
	uint8 month;
	uint16 daysInYear, currentDays;
	uint32 relativeYear;
	int64 currentTimeStamp;

	const uint64 MILLISECONDS_PER_MINUTE = 60 * 1000;
	const uint64 MILLISECONDS_PER_HOUR = 60 * MILLISECONDS_PER_MINUTE;
	const uint64 MILLISECONDS_PER_DAY = 24 * MILLISECONDS_PER_HOUR;
	const uint64 MILLISECONDS_PER_YEAR = 365 * MILLISECONDS_PER_DAY;

	currentTimeStamp = this->timeStamp;
	currentTimeStamp -= this->timeZoneBias * MILLISECONDS_PER_MINUTE;

	//year
	relativeYear = (uint32)(currentTimeStamp / MILLISECONDS_PER_YEAR);
	refDateTimeInfo.year = relativeYear + 1970;
	currentTimeStamp -= relativeYear * MILLISECONDS_PER_YEAR;

	//check leap years
	currentTimeStamp -= GetNumberOfElapsedLeapYears(relativeYear) * MILLISECONDS_PER_DAY;

	//days in current year
	daysInYear = (uint16)(currentTimeStamp / MILLISECONDS_PER_DAY);
	currentTimeStamp -= daysInYear * MILLISECONDS_PER_DAY;

	//month and day in month
	currentDays = 0;
	for(month = 0; currentDays < daysInYear; month++)
	{
		refDateTimeInfo.day = daysInYear - currentDays;
		NOT_IMPLEMENTED_ERROR; //TODO: next line
		//currentDays += calendar.GetNumberOfDaysInMonth(month, refDateTimeInfo.year);
	}
	refDateTimeInfo.month = month - 1;

	//weekday
	//1970-01-01 was a thursday thus 4
	refDateTimeInfo.weekDay = ((this->timeStamp / MILLISECONDS_PER_DAY) + 4) % 7;

	//hour
	refDateTimeInfo.hour = (uint8)(currentTimeStamp / MILLISECONDS_PER_HOUR);
	currentTimeStamp -= refDateTimeInfo.hour * MILLISECONDS_PER_HOUR;

	//min
	refDateTimeInfo.min = (uint8)(currentTimeStamp / MILLISECONDS_PER_MINUTE);
	currentTimeStamp -= refDateTimeInfo.min * MILLISECONDS_PER_MINUTE;

	//sec
	refDateTimeInfo.sec = (uint8)(currentTimeStamp / 1000);
	currentTimeStamp -= refDateTimeInfo.sec * 1000;

	//ms
	refDateTimeInfo.milliseconds = (uint16)currentTimeStamp;
}
*/