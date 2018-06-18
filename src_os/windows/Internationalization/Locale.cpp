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
#include <Std++/Internationalization/Locale.hpp>
//Global
#include <Windows.h>
//Namespaces
using namespace StdPlusPlus;

#include <Std++/Streams/StdOut.hpp>
//Public methods
TimeZone Locale::GetTimeZone() const
{
	DYNAMIC_TIME_ZONE_INFORMATION dtzi;
	for (DWORD index = 0; EnumDynamicTimeZoneInformation(index, &dtzi) == 0; index++) //very poor doc for EnumDynamicTimeZoneInformation, apparent 0 means success
	{
		stdOut << (uint32)index << endl;
		stdOut << String::CopyRawString((uint16 *)dtzi.StandardName) << endl;
		stdOut << String::CopyRawString((uint16 *)dtzi.TimeZoneKeyName) << endl;
	}
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return TimeZone();
}

//Class functions
Locale Locale::GetUserLocale()
{
	wchar_t name[LOCALE_NAME_MAX_LENGTH];
	GetUserDefaultLocaleName(name, LOCALE_NAME_MAX_LENGTH);

	return Locale(String::CopyRawString((uint16 *)name));
}