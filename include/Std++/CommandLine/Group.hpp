/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include "Argument.hpp"
#include "Option.hpp"

namespace StdXX::CommandLine
{
	class Group : public Argument
	{
	public:
		//Constructor
		inline Group(const String& name, const String& description) : Argument(name, description)
		{
		}

		//Properties
		inline const BinaryTreeMap<String, const Option*>& LongOptionsMap() const
		{
			return this->longNameOptionsMap;
		}

		inline const DynamicArray<const Argument*>& PositionalArguments() const
		{
			return this->positionalArgs;
		}

		//Methods
		void Match(MatchResult &result) const override;
		void MatchOptions(MatchResult& result) const;

		//Inline
		inline void AddOption(const Option& option)
		{
			ASSERT(!this->shortNameOptionsMap.Contains(option.ShortName()), u8"Double short name detected");
			ASSERT(!this->longNameOptionsMap.Contains(option.LongName()), u8"Double long name detected");

			this->shortNameOptionsMap.Insert(option.ShortName(), &option);
			this->longNameOptionsMap.Insert(option.LongName(), &option);
		}

		inline void AddPositionalArgument(const Argument& argument)
		{
			this->positionalArgs.Push(&argument);
		}

	protected:
		//Members
		BinaryTreeMap<uint32, const Option*> shortNameOptionsMap;
		BinaryTreeMap<String, const Option*> longNameOptionsMap;
		DynamicArray<const Argument*> positionalArgs;
	};
}