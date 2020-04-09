/*
* Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
using namespace StdXX;

//Class functions
DateTime DateTime::FromUnixTimeStamp(int64 timeStamp)
{
	//constants
	const uint16 SECONDS_PER_MINUTE = 60;
	const uint16 SECONDS_PER_HOUR = 60 * SECONDS_PER_MINUTE;
	const uint32 SECONDS_PER_DAY = 24 * SECONDS_PER_HOUR;

	const int32 nSeconds = timeStamp % SECONDS_PER_DAY;
	const int64 nDays = (timeStamp / SECONDS_PER_DAY) + (nSeconds < 0 ? -1 : 0);
	return DateTime(StdXX::Date::Epoch.AddDays(nDays), StdXX::Time().AddSecs(nSeconds));
}

DateTime DateTime::ParseISOString(const String &string)
{
	DynamicArray<String> parts = string.Split(u8" ");
	ASSERT_EQUALS(parts.GetNumberOfElements(), 2);

	return DateTime(Date::ParseISOString(parts[0]), Time::ParseISOString(parts[1]));
}
