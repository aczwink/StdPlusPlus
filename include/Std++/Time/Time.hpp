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
#include "../Definitions.h"
#include <Std++/Containers/Strings/String.hpp>

namespace StdXX
{
	/**
	* Represents times within a 24 hour format.
	*/
	class STDPLUSPLUS_API Time
	{
	public:
		//Constructors
		inline Time() : millisecs(0)
		{
		}

		inline Time(uint8 hour, uint8 min = 0, uint8 secs = 0, uint16 millisecs = 0)
		{
			this->Set(hour, min, secs, millisecs);
		}

		//Operators
		inline bool operator==(const Time& other) const
		{
			return this->millisecs == other.millisecs;
		}

		inline bool operator<(const Time &other) const
		{
			return this->millisecs < other.millisecs;
		}

		inline bool operator>=(const Time &other) const
		{
			return this->millisecs >= other.millisecs;
		}

		//Properties
		inline uint8 Hours() const
		{
			return this->millisecs / 1000 / 60 / 60;
		}

		inline uint16 MilliSeconds() const
		{
			return this->millisecs % 1000;
		}

		inline uint8 Minutes() const
		{
			return (this->millisecs / 1000 / 60) % 60;
		}

		inline uint8 Seconds() const
		{
			return (this->millisecs / 1000) % 60;
		}

		//Methods
		void Set(uint8 hour, uint8 min, uint8 secs, uint16 millisecs);

		//Inline
		inline Time AddSecs(int32 s) const
		{
			return this->AddMSecs(s * 1000);
		}

		inline Time AddMSecs(int32 ms) const
		{
			const uint32 msecsPerDay = (24 * 60 * 60 * 1000);
			Time t;
			t.millisecs = (this->millisecs + ms) % msecsPerDay;
			return t;
		}

		inline uint32 GetMilliSecondsSinceStartOfDay() const
		{
			return this->millisecs;
		}
		/**
		* Format time according to ISO 8601 i.e. "hh:mm:ss.sss".
		*/
		inline String ToISOString() const
		{
			return String::Number(this->Hours(), 10, 2) + u8":" + String::Number(this->Minutes(), 10, 2) + u8":" +
				   String::Number(this->Seconds(), 10, 2) + u8"." + String::Number(this->MilliSeconds(), 10, 3);
		}

	private:
		//Members
		/*
		Invariant: millisecs < 24 * 60 * 60 * 1000 = 86,400,000 i.e. max allowed value is 23:59:59.999
		*/
		uint32 millisecs;
	};
}