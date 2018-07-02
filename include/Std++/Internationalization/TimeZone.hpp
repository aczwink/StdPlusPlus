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
#pragma once
//Local
#include <Std++/Definitions.h>
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Time/DateTime.hpp>

namespace StdPlusPlus
{
	class STDPLUSPLUS_API TimeZone
	{
	public:
		//Constructor
		/**
		* 'timeZoneIdentifier' is currently an os-dependent (unfortunately) name for a time zone.
		* It is strongly discouraged to call this constructor directly, because the same identifier will likely not work on another platform.
		* Until that is fixed, try to use only the GetUserLocalTimeZone and GetUTCTimeZone functions, as these are cross-platform.
		* 
		* There is no direct relation between a country and a time zone (for instance the U.S. have several ones).
		* That means that we can't map a locale to exactly one time zone.
		*/
		TimeZone(const String &timeZoneIdentifier);

		//Methods
		/**
		* If 'dt' is in UTC-time, then the result will be the exact same time but in this time zone (i.e. the time will be offset by the time zones difference to UTC).
		*/
		DateTime Translate(const DateTime &dt) const;

		//Functions
		static TimeZone GetUserLocalTimeZone();

	private:
		//Members
		void *osHandle;
	};
}