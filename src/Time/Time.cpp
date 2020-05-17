/*
* Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Time/Time.hpp>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Signed.hpp>
//Namespaces
using namespace StdXX;

//Public methods
void Time::Set(uint8 hour, uint8 min, uint8 secs, uint16 millisecs, uint16 microsecs, uint16 nanosecs)
{
	ASSERT(hour < 24, u8"Invalid hour value");
	ASSERT(min < 60, u8"Invalid minute value");
	ASSERT(secs < 60, u8"Invalid seconds value");
	ASSERT(millisecs < 1000, u8"Invalid milliseconds value");
	ASSERT(microsecs < 1000, u8"Invalid milliseconds value");
	ASSERT(nanosecs < 1000, u8"Invalid milliseconds value");

	this->nanoseconds = nanosecs + 1000_i64 * (microsecs + 1000_i64 * (millisecs + 1000_i64 * (secs + 60_i64 * (min + 60_i64 * hour) ) ) );
}

//Class functions
Time Time::ParseISOString(const String &string)
{
	DynamicArray<String> parts = string.Split(u8":");
	ASSERT_EQUALS(3_u32, parts.GetNumberOfElements());

	DynamicArray<String> subParts = parts[2].Split(u8".");
	ASSERT_EQUALS(2_u32, subParts.GetNumberOfElements());

	ASSERT(subParts[1].EndsWith(u8"Z"), u8"TODO: DO THIS CORRECTLY");
	String fractional = subParts[1].SubString(0, subParts[1].GetLength()-1);
	uint64 frac = fractional.ToUInt();
	if(fractional.GetLength() > 9)
		frac /= Math::Power(10_u64, fractional.GetLength() - 9_u64);
	else if(fractional.GetLength() < 9)
		frac *= Math::Power(10_u64, 9_u64 - fractional.GetLength());

	return Time(parts[0].ToUInt(), parts[1].ToUInt(), subParts[0].ToUInt()).AddNanoseconds(frac);
}
