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
#include <Std++/Time/Time.hpp>
//Global
#include <Windows.h>
//Local
#include "TimeConstants.h"
//Namespaces
using namespace StdPlusPlus;

//Constructor
Time::Time()
{
	FILETIME ft;
	TIME_ZONE_INFORMATION timeZoneInfo;
	
	GetSystemTimePreciseAsFileTime(&ft);
	this->timeStamp = FileTimeToMilliseconds(ft);
	GetTimeZoneInformation(&timeZoneInfo);

	this->timeZoneBias = (int8)timeZoneInfo.Bias;
}

Time::Time(int64 timeStamp)
{
	TIME_ZONE_INFORMATION timeZoneInfo;

	GetTimeZoneInformation(&timeZoneInfo);

	this->timeStamp = timeStamp;
	this->timeZoneBias = (int8)timeZoneInfo.Bias;
}