/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Tuple.hpp>

namespace StdXX::CommandLine
{
	//Forward declarations
	class MatchResult;

	class Option
	{
	public:
		//Constructor
		inline Option(uint32 shortName, const String& longName, const String& description)
				: shortName(shortName), longName(longName), description(description)
		{
		}

		//Properties
		inline const String& Description() const
		{
			return this->description;
		}

		inline const String& LongName() const
		{
			return this->longName;
		}

		inline uint32 ShortName() const
		{
			return this->shortName;
		}

		//Methods
		virtual void Match(MatchResult& result) const;
		virtual String ToString() const;

	protected:
		//Methods
		bool MatchesName(MatchResult& result) const;

	private:
		//Members
		uint32 shortName;
		String longName;
		String description;
	};
}