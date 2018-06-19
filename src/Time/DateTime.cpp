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

//Local functions
static uint32 GetNumberOfElapsedLeapYears(uint32 year) //since year 0
{
	int32 nLeapYears;

	nLeapYears = year / 4;
	nLeapYears -= year / 100;
	nLeapYears += (year + 300) / 400;

	return nLeapYears;
}

//Class functions
DateTime DateTime::FromUnixTimeStampWithMilliSeconds(int64 timeStamp)
{
	int64 currentTimeStamp = timeStamp;

	//constants
	const uint16 MILLISECONDS_PER_MINUTE = 60 * 1000;
	const uint32 MILLISECONDS_PER_HOUR = 60 * MILLISECONDS_PER_MINUTE;
	const uint32 MILLISECONDS_PER_DAY = 24 * MILLISECONDS_PER_HOUR;
	const uint64 MILLISECONDS_PER_YEAR = 365 * uint64(MILLISECONDS_PER_DAY);

	//year
	int32 relativeYear = (int32)(currentTimeStamp / MILLISECONDS_PER_YEAR);
	currentTimeStamp -= relativeYear * MILLISECONDS_PER_YEAR;
	currentTimeStamp -= GetNumberOfElapsedLeapYears(relativeYear) * MILLISECONDS_PER_DAY; //leap years
	
	//days in current year
	uint16 daysInYear = (uint16)(currentTimeStamp / MILLISECONDS_PER_DAY);
	currentTimeStamp -= uint64(daysInYear) * MILLISECONDS_PER_DAY;

	//month and day in month
	uint16 currentDays = 0;
	uint8 month, day;
	for (month = 0; currentDays < daysInYear; month++)
	{
		day = daysInYear - currentDays;
		currentDays += Date::GetNumberOfDaysInMonth(month, relativeYear + 1970);
	}

	//hour
	uint8 hour = (uint8)(currentTimeStamp / MILLISECONDS_PER_HOUR);
	currentTimeStamp -= hour * MILLISECONDS_PER_HOUR;

	//min
	uint8 min = (uint8)(currentTimeStamp / MILLISECONDS_PER_MINUTE);
	currentTimeStamp -= min * MILLISECONDS_PER_MINUTE;

	//sec
	uint8 sec = (uint8)(currentTimeStamp / 1000);
	currentTimeStamp -= sec * 1000;
	
	return DateTime(Date(relativeYear + 1970, month, day+1), Time(hour, min, sec, (uint16)currentTimeStamp)); //rest of currentTimeStamp is ms
}

/*
//Private methods
void Time::CalcDateTimeInfo(SDateTimeInfo &refDateTimeInfo) const
{
	currentTimeStamp -= this->timeZoneBias * MILLISECONDS_PER_MINUTE;

	//weekday
	//1970-01-01 was a thursday thus 4
	refDateTimeInfo.weekDay = ((this->timeStamp / MILLISECONDS_PER_DAY) + 4) % 7;
}
*/