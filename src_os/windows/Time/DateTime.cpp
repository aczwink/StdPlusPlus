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
//Global
#include <Windows.h>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Internationalization/Locale.hpp>
//Namespaces
using namespace StdXX;

//Local functions
static int64 FileTimeToUnixTimeStampWithMilliSeconds(const FILETIME &ft)
{
	int64 timeStamp;

	const uint64 FILETIME_TO_EPOCH_BIAS = 116444736000000000i64; //Number of 100 nanosecond units from 1/1/1601 to 1/1/1970

	timeStamp = ((uint64)ft.dwHighDateTime << 32) | ((uint64)ft.dwLowDateTime);
	timeStamp -= FILETIME_TO_EPOCH_BIAS;
	timeStamp /= (10 * 1000); //scale to ms

	return timeStamp;
}

//Class functions
DateTime DateTime::Now()
{
	FILETIME ft;
#if WINVER >= _WIN32_WINNT_WIN8 //0x602
	GetSystemTimePreciseAsFileTime(&ft);
#else
	GetSystemTimeAsFileTime(&ft);
#endif
	//ft is UTC
	return DateTime::FromUnixTimeStampWithMilliSeconds(FileTimeToUnixTimeStampWithMilliSeconds(ft));
}

/*
//Constructor
Time::Time()
{
	TIME_ZONE_INFORMATION timeZoneInfo;
	GetTimeZoneInformation(&timeZoneInfo);
	this->timeZoneBias = (int8)timeZoneInfo.Bias;
}
*/