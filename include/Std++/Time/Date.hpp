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
#include "WeakDate.hpp"
#include <Std++/Debug.hpp>
#include <Std++/Signed.hpp>

namespace StdXX
{
	/**
	* Represents dates within the Gregorian calendar.
	*
	* This class represents only valid dates!
	*/
	class STDPLUSPLUS_API Date
	{
	public:
		//Constants
		static const Date Epoch;

		//Constructors
		Date(int64 year, uint8 month, uint8 day);
		Date(const Date&) = default;

		//Operators
		inline bool operator<(const Date &other) const
		{
			return this->deltaDays < other.deltaDays;
		}

		inline bool operator>(const Date &other) const
		{
			return other < *this;
		}

		inline bool operator==(const Date &other) const
		{
			return this->deltaDays == other.deltaDays;
		}

		//Properties
		inline uint8 Day() const
		{
			return this->ToWeakDate().day;
		}

		inline uint8 Month() const
		{
			return this->ToWeakDate().month;
		}

		inline int64 NumberOfDaysSinceEpoch() const
		{
			return this->deltaDays;
		}

		int64 Year() const;

		//Functions
		/**
		 * Includes fromYear but excludes toYear.
		 * Negative if toYear is less than fromYear.
		 * @param fromYear
		 * @param toYear
		 * @return
		 */
		static int64 ComputeNumberOfLeapYears(int64 fromYear, int64 toYear);
		static Date ParseISOString(const String& string);

		//Inline
		inline Date AddDays(int64 days) const
		{
			return Date(this->deltaDays + days);
		}
		/*
		Returns the number of days 'd', such that *this + 'd' = other.
		That means that the result is negative, if *this > other
		*/
		inline int64 GetDaysTo(const Date &other) const
		{
			return other.deltaDays - this->deltaDays;
		}
		/*
		Returns the day of the week (1 = Monday to 7 = Sunday as of ISO 8601)
		In case the date is invalid, 0 is returned
		*/
		inline uint8 GetWeekDay() const
		{
			//1970-01-01 was a thursday, i.e. 4. As weekday is 1-based, we need to offset by 3
			return 1 + ( this->deltaDays + 3) % 7;
		}
		
		/**
		* Format date according to ISO 8601 i.e. "YYYY-MM-DD".
		*/
		inline String ToISOString() const
		{
			return this->ToWeakDate().ToISOString();
		}

		//Inline functions
		inline static Date MinValue()
		{
			return Date(Signed<int64>::Max());
		}

	private:
		//Members
		/*
		Days since epoch 1970-01-01
		*/
		int64 deltaDays;

		//Constructor
		inline Date(int64 deltaDays) : deltaDays(deltaDays)
		{
		}

		//Methods
		WeakDate ToWeakDate() const;
	};
}