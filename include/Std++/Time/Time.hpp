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
#pragma once
//Local
#include <Std++/Mathematics.hpp>
#include <Std++/Containers/Strings/String.hpp>

namespace StdXX
{
	/**
	* Represents times within a 24 hour format with nanosecond precision, which resembles current processor precision.
	*/
	class STDPLUSPLUS_API Time
	{
	public:
		//Constants
		static const constexpr uint64 MaxValue = 24_u64 * 60 * 60 * 1000 * 1000 * 1000;

		//Constructors
		inline Time() : nanoseconds(0)
		{
		}

		inline Time(uint8 hour, uint8 min = 0, uint8 secs = 0, uint16 millisecs = 0, uint16 microsecs = 0, uint16 nanosecs = 0)
		{
			this->Set(hour, min, secs, millisecs, microsecs, nanosecs);
		}

		Time(const Time&) = default;

		//Operators
		inline bool operator==(const Time& other) const
		{
			return this->nanoseconds == other.nanoseconds;
		}

		inline bool operator<(const Time &other) const
		{
			return this->nanoseconds < other.nanoseconds;
		}

		inline bool operator>=(const Time &other) const
		{
			return this->nanoseconds >= other.nanoseconds;
		}

		//Properties
		inline uint8 Hours() const
		{
			return this->nanoseconds / 1000 / 1000 / 1000 / 60 / 60;
		}

		inline uint16 Milliseconds() const
		{
			return (this->nanoseconds / 1000 / 1000) % 1000;
		}

		inline uint64 NanosecondsSinceStartOfDay() const
		{
			return this->nanoseconds;
		}

		inline uint64 NanosecondsSinceStartOfSecond() const
		{
			return this->nanoseconds % 1000000000;
		}

		inline uint8 Minutes() const
		{
			return (this->nanoseconds / 1000 / 1000 / 1000 / 60) % 60;
		}

		inline uint8 Seconds() const
		{
			return this->SecondsSinceStartOfDay() % 60;
		}

		inline uint32 SecondsSinceStartOfDay() const
		{
			return this->nanoseconds / 1000 / 1000 / 1000;
		}

		//Methods
		void Set(uint8 hour, uint8 min, uint8 secs, uint16 millisecs, uint16 microsecs, uint16 nanosecs);

		//Class functions
		inline static Time FromNanosecondsSinceStartOfDay(uint64 nanoseconds)
		{
			ASSERT(nanoseconds < MaxValue, u8"Invalid value.");
			Time t;
			t.nanoseconds = nanoseconds;
			return t;
		}
		
		static Time ParseISOString(const String& string);

		//Inline
		inline Time AddMicroseconds(int64 microseconds) const
		{
			return this->AddNanoseconds(microseconds * 1000);
		}

		inline Time AddMilliseconds(int32 ms) const
		{
			return this->AddMicroseconds(ms * 1000_u64);
		}

		inline Time AddNanoseconds(int64 nanoseconds) const
		{
			Time t;
			t.nanoseconds = this->nanoseconds;
			if(nanoseconds != 0)
			{
				const int64 nsecsPerDay = MaxValue * Math::Sign(nanoseconds);
				t.nanoseconds = (t.nanoseconds + nanoseconds) % nsecsPerDay;
			}
			return t;
		}

		inline Time AddSecs(int32 s) const
		{
			return this->AddMilliseconds(s * 1000);
		}

		/**
		* Format time according to ISO 8601 i.e. "hh:mm:ss.sssssssssZ".
		*/
		inline String ToISOString() const
		{
			return String::Number(this->Hours(), 10, 2) + u8":" + String::Number(this->Minutes(), 10, 2) + u8":" +
				   String::Number(this->Seconds(), 10, 2) + u8"." + String::Number(this->nanoseconds % 1000000000, 10, 9)
				   + u8"Z";
		}

	private:
		//Members
		/*
		Invariant: nanoseconds < 24 * 60 * 60 * 1000 * 1000 * 1000 = 86,400,000,000,000 i.e. max allowed value is 23:59:59.999999999
		*/
		uint64 nanoseconds;
	};
}