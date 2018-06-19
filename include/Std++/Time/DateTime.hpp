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
#include <Std++/Internationalization/TimeZone.hpp>

namespace StdPlusPlus
{
    class STDPLUSPLUS_API DateTime
    {
		/*
        struct SDateTimeInfo
        {
            uint8 weekDay;
        };
    private:
        //Members
        int64 timeStamp; //number of milli seconds since the Epoch (1970-01-01 00:00 UTC+local)
        int32 timeZoneBias; //UTC+x in minutes

        //Methods
        void CalcDateTimeInfo(SDateTimeInfo &refDateTimeInfo) const;*/

    public:
        //Constructor
		inline DateTime(const Date &date, const Time &time)
			: date(date), time(time)
		{
		}

        //Inline
		/**
		* Format date and time according to ISO 8601 i.e. "YYYY-MM-DD hh:mm:ss.sss".
		*/
		inline String ToISOString() const
		{
			return this->date.ToISOString() + u8" " + this->time.ToISOString();
		}

		inline DateTime ToTimeZone(const TimeZone &other) const
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return DateTime(*this);
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
        }

        inline uint32 GetYear() const
        {
            SDateTimeInfo dti;

            this->CalcDateTimeInfo(dti);

            return dti.year;
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