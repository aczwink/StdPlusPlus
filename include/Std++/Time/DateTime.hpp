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

        //Inline
		inline DateTime AddMilliSeconds(int64 milliSeconds) const
		{
			const uint32 msecsPerDay = (24 * 60 * 60 * 1000);
			int32 ms = (this->time.GetMilliSecondsSinceStartOfDay() + milliSeconds) % msecsPerDay;
			int64 deltaDays = (milliSeconds - ms) / msecsPerDay;

			return DateTime(this->date.AddDays(deltaDays), this->time.AddMSecs(ms));
		}

		inline DateTime AddMinutes(int64 minutes) const
		{
			return this->AddSeconds(minutes * 60);
		}

		inline DateTime AddSeconds(int64 seconds) const
		{
			return this->AddMilliSeconds(seconds * 1000);
		}
		/**
		* Format date and time according to ISO 8601 i.e. "YYYY-MM-DD hh:mm:ss.sss".
		*/
		inline String ToISOString() const
		{
			return this->date.ToISOString() + u8" " + this->time.ToISOString();
		}

		inline uint32 GetYear() const
		{
			return this->date.GetYear();
		}

        /*inline uint8 GetDay() const
        {
            SDateTimeInfo dti;

            this->CalcDateTimeInfo(dti);

            return dti.day;
        }

        inline uint8 GetHour() const
        {
            SDateTimeInfo dti;

            this->CalcDateTimeInfo(dti);

            return dti.hour;
        }

        inline uint16 GetMilliSeconds() const
        {
            SDateTimeInfo dti;

            this->CalcDateTimeInfo(dti);

            return dti.milliseconds;
        }

        inline uint16 GetMinute() const
        {
            SDateTimeInfo dti;

            this->CalcDateTimeInfo(dti);

            return dti.min;
        }

        inline uint8 GetMonth() const
        {
            SDateTimeInfo dti;

            this->CalcDateTimeInfo(dti);

            return dti.month;
        }

        inline uint8 GetSeconds() const
        {
            SDateTimeInfo dti;

            this->CalcDateTimeInfo(dti);

            return dti.sec;
        }

        inline int64 GetTimeStamp() const
        {
            return this->timeStamp;
        }

        inline uint8 GetWeekDay() const
        {
            SDateTimeInfo dti;

            this->CalcDateTimeInfo(dti);

            return dti.weekDay;
        }*/

		//Functions
		/**
		* timeStamp is assumed to be in UTC+0
		*/
		static DateTime FromUnixTimeStampWithMilliSeconds(int64 timeStamp);
		/*
		* Returns the current system time in UTC time-zone.
		* Note that if the system time is different from the current "real-world" wall-clock time, so is the result.
		*/
		static DateTime Now();

	private:
		//Members
		Date date;
		Time time;
    };
}