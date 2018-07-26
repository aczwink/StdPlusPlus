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
#include "../Definitions.h"
#include <Std++/Containers/Strings/String.hpp>

namespace StdXX
{
	/**
	* Represents times within a 24 hour format.
	*/
	class STDPLUSPLUS_API Time
	{
		friend class DateTime;
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
		inline bool operator<(const Time &other) const
		{
			return this->millisecs < other.millisecs;
		}

		inline bool operator>=(const Time &other) const
		{
			return this->millisecs >= other.millisecs;
		}

		//Methods
		void Set(uint8 hour, uint8 min, uint8 secs, uint16 millisecs);

		//Inline
		inline uint8 GetHour() const
		{
			return this->millisecs / 1000 / 60 / 60;
		}

		inline uint16 GetMilliSecond() const
		{
			return this->millisecs % 1000;
		}

		inline uint8 GetMinute() const
		{
			return (this->millisecs / 1000 / 60) % 60;
		}

		inline uint8 GetSecond() const
		{
			return (this->millisecs / 1000) % 60;
		}
		/**
		* Format time according to ISO 8601 i.e. "hh:mm:ss.sss".
		*/
		inline String ToISOString() const
		{
			return String::Number(this->GetHour(), 10, 2) + u8":" + String::Number(this->GetMinute(), 10, 2) + u8":" +
				String::Number(this->GetSecond(), 10, 2) + u8"." + String::Number(this->GetMilliSecond(), 10, 3);
		}

	private:
		//Members
		/*
		Invariant: millisecs < 24 * 60 * 60 * 1000 = 86,400,000 i.e. max allowed value is 23:59:59.999
		*/
		uint32 millisecs;
	};
}