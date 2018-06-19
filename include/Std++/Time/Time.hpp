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

namespace StdPlusPlus
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

		inline Time(uint8 hour, uint8 min, uint8 secs, uint16 millisecs)
		{
			this->Set(hour, min, secs, millisecs);
		}

		//Methods
		void Set(uint8 hour, uint8 min, uint8 secs, uint16 millisecs);

		//Inline
		/**
		* Format time according to ISO 8601 i.e. "hh:mm:ss.sss".
		*/
		inline String ToISOString() const
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return "";
		}

	private:
		//Members
		/*
		Invariant: millisecs < 24 * 60 * 60 * 1000 = 86,400,000 i.e. max allowed value is 23:59:59.999
		*/
		uint32 millisecs;
	};
}