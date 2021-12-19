/*
* Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
//Namespaces
using namespace StdXX;

//Constructor
TimeZone::TimeZone(const String &timeZoneIdentifier)
{
    if(timeZoneIdentifier == tzname[0])
        this->osHandle = tzname[0];
    else
        NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Public methods
DateTime TimeZone::Translate(const DateTime &dt) const
{
    if(this->osHandle == tzname[0])
        return dt.AddSeconds(-timezone);

    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return DateTime(Date(1, 1, 1), StdXX::Time());
}

//Class functions
TimeZone TimeZone::GetUserLocalTimeZone()
{
    tzset();

    return String::CopyRawString(tzname[0]);
}