/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include "../Definitions.h"

namespace ACStdLib
{
    class ACSTDLIB_API CTime
    {
        struct SDateTimeInfo
        {
            uint8 day;
            uint8 month;
            uint32 year;

            uint8 hour;
            uint8 min;
            uint8 sec;
            uint16 milliseconds;

            uint8 weekDay;
        };
    private:
        //Members
        int64 timeStamp; //number of milli seconds since the Epoch (1970-01-01 00:00 UTC+local)
        int32 timeZoneBias; //UTC+x in minutes

        //Methods
        void CalcDateTimeInfo(SDateTimeInfo &refDateTimeInfo) const;

    public:
        //Constructors
        CTime(); //current time in UTC+0
        CTime(int64 timeStamp);

        //Inline
        inline uint8 GetDay() const
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
        }
    };
}