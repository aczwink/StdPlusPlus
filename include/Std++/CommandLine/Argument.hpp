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

	class Argument
	{
	public:
		//Constructor
		inline Argument(const String& name, const String& description)
			: name(name), description(description)
		{
		}

		//Abstract
		virtual void Match(MatchResult &result) const = 0;

		//Methods
		virtual String ToString() const;

		//Properties
		inline const String& Description() const
		{
			return this->description;
		}

		inline const String& Name() const
		{
			return this->name;
		}

	private:
		//Members
		String name;
		String description;
	};
}