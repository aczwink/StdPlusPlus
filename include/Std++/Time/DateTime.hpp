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
#pragma once
//Local
#include "../Definitions.h"
#include "Date.hpp"
#include "Time.hpp"

namespace StdXX
{
    class STDPLUSPLUS_API DateTime
    {
	public:
        //Constructor
		inline DateTime(const Date &date, const Time &time)
			: date(date), time(time)
		{
		}

		//Operators
		inline bool operator==(const DateTime& other) const
		{
        	return (this->date == other.date) && (this->time == other.time);
		}

		inline bool operator!=(const DateTime& other) const
		{
        	return !(*this == other);
		}

		inline bool operator<(const DateTime &other) const
		{
			if (this->date < other.date)
				return true;
			if(this->date == other.date)
				return this->time < other.time;
			return false;
		}

		inline bool operator>=(const DateTime &other) const
		{
			if (this->date > other.date)
				return true;
			if (this->date == other.date)
				return this->time >= other.time;
			return false;
		}

		//Properties
		inline const class Date& Date() const
		{
			return this->date;
		}

		inline const class Time& Time() const
		{
        	return this->time;
		}

        //Inline
        inline DateTime AddMicroseconds(int64 microseconds) const
		{
        	return this->AddNanoseconds(microseconds * 1000);
		}

		inline DateTime AddMilliSeconds(int64 milliSeconds) const
		{
			return this->AddMicroseconds(milliSeconds * 1000);
		}

		inline DateTime AddMinutes(int64 minutes) const
		{
			return this->AddSeconds(minutes * 60);
		}

		inline DateTime AddNanoseconds(int64 nanoseconds) const
		{
			//we check how many days will overflow
			const int64 deltaDays = (this->time.NanosecondsSinceStartOfDay() + nanoseconds) / Time::MaxValue;
			const int64 ns = (this->time.NanosecondsSinceStartOfDay() + nanoseconds) % Time::MaxValue;

			return DateTime(this->date.AddDays(deltaDays), Time::FromNanosecondsSinceStartOfDay(ns));
		}

		inline DateTime AddSeconds(int64 seconds) const
		{
			return this->AddMilliSeconds(seconds * 1000);
		}

		inline const class Time& GetTime() const
		{
			return this->time;
		}

		/**
		* Format date and time according to ISO 8601 i.e. "YYYY-MM-DD hh:mm:ss.sss".
		*/
		inline String ToISOString() const
		{
			return this->date.ToISOString() + u8" " + this->time.ToISOString();
		}

		//Functions
	    /**
		* timeStamp is assumed to be in UTC+0
		*/
	    static DateTime FromUnixTimeStamp(int64 timeStamp);

		static DateTime MinValue()
		{
			return {Date::MinValue(), {}};
		}

		/**
		 * Returns the current system time in UTC+0.
		 * Note that if the system time is different from the current "real-world" wall-clock time, so is the result.
		 */
		static DateTime Now();
		static DateTime ParseISOString(const String& string);

	private:
		//Members
		class Date date;
		class Time time;
    };
}