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
#include <Std++/Internationalization/TimeZone.hpp>
//Global
#include <Windows.h>
//Namespaces
using namespace StdXX;

//Local functions
static DateTime TimeZoneDescriptionRelativeSystemTimeToDateTime(const SYSTEMTIME &systemTime, int64 year)
{
	uint8 day;
	//find the first week-day
	for (day = 1; day <= 7; day++)
	{
		Date current(year, systemTime.wMonth, day);

		if ((current.GetWeekDay() % 7) == systemTime.wDayOfWeek)
			break;
	}
	day = day + 7 * (systemTime.wDay - 1);
	if ((systemTime.wDay == 5) && (day > WeakDate::GetNumberOfDaysInMonth(systemTime.wMonth, year))) //means the last time the day occured in the month. A weekday occurs either 4 or 5 times a month
		day -= 7;

	return DateTime(Date(year, systemTime.wMonth, day), Time(systemTime.wHour, systemTime.wMinute)); //only hour and minute are valid according to MSDN
}

//Constructor
TimeZone::TimeZone(const String &timeZoneIdentifier)
{
#if WINVER >= _WIN32_WINNT_WIN8
	DYNAMIC_TIME_ZONE_INFORMATION dtzi;
	ASSERT(timeZoneIdentifier.GetLength() < sizeof(dtzi.TimeZoneKeyName) / sizeof(dtzi.TimeZoneKeyName[0]), u8"Illegal time zone identifier");
	for (DWORD index = 0; EnumDynamicTimeZoneInformation(index, &dtzi) == 0; index++) //very poor doc for EnumDynamicTimeZoneInformation, apparently 0 means success
	{
		if (MemCmp(timeZoneIdentifier.ToUTF16().GetRawData(), dtzi.TimeZoneKeyName, timeZoneIdentifier.GetSize()) == 0)
		{
			this->osHandle = (void *)index;
			return;
		}
	}
#else
	NOT_IMPLEMENTED_ERROR; //TODO: read time zones from HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Time Zones
#endif

	NOT_IMPLEMENTED_ERROR; //TOOD: Illegal time zone identifier
}

//Public methods
DateTime TimeZone::Translate(const DateTime &dt) const
{
	TIME_ZONE_INFORMATION tzi;
#if WINVER >= _WIN32_WINNT_WIN8
	DYNAMIC_TIME_ZONE_INFORMATION dtzi;
	EnumDynamicTimeZoneInformation((DWORD)this->osHandle, &dtzi);
	GetTimeZoneInformationForYear(dt.GetYear(), &dtzi, &tzi);
#else
	NOT_IMPLEMENTED_ERROR; //TODO: Time zone from registry key (see above)
#endif

	if (tzi.DaylightDate.wMonth != 0)
	{
		//DST is used in that time zone
		ASSERT(tzi.DaylightDate.wYear == 0, u8"As GetTimeZoneInformationForYear gives an information on one year, this should always be a relative date?!");
		//relative-date
		DateTime dst = TimeZoneDescriptionRelativeSystemTimeToDateTime(tzi.DaylightDate, dt.GetYear());
		DateTime std = TimeZoneDescriptionRelativeSystemTimeToDateTime(tzi.StandardDate, dt.GetYear());
		bool isDST = dt >= dst && dt < std; //std should be > then dst

		if (isDST)
			return dt.AddMinutes( -(tzi.Bias + tzi.DaylightBias) );
		return dt.AddMinutes(-(tzi.Bias + tzi.StandardBias));
	}
	else
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return DateTime::FromUnixTimeStampWithMilliSeconds(0);
}

//Class functions
TimeZone TimeZone::GetUserLocalTimeZone()
{
	DYNAMIC_TIME_ZONE_INFORMATION dtzi;
	GetDynamicTimeZoneInformation(&dtzi);

	return TimeZone(String::CopyRawString((uint16 *)dtzi.TimeZoneKeyName));
}