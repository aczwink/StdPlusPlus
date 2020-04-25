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
#include "Argument.hpp"
#include "Group.hpp"

namespace StdXX::CommandLine
{
	class SubCommandArgument : public Argument
	{
	public:
		//Constructor
		inline SubCommandArgument(const String& name, const String& description) : Argument(name, description)
		{
		}

		//Properties
		inline const DynamicArray<const Group*>& Commands() const
		{
			return this->commands;
		}

		//Methods
		void Match(MatchResult &result) const override;
		String ToString() const override;

		//Inline
		inline void AddCommand(const Group& group)
		{
			this->commands.Push(&group);
		}

	private:
		//Members
		DynamicArray<const Group*> commands;
	};
}