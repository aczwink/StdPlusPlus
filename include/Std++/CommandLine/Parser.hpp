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
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "Option.hpp"
#include "Argument.hpp"
#include "Group.hpp"
#include "MatchResult.hpp"
#include "SubCommandArgument.hpp"

namespace StdXX::CommandLine
{
	class Parser : public Group
	{
	public:
		//Constructor
		inline Parser(const String& programName) : Group(String(), String()), programName(programName),
			helpOption(u8'h', u8"help", u8"Show this text or the help of a given subcommand")
		{
		}

		//Properties
		inline const MatchResult& ParseResult() const
		{
			return *this->matchResult;
		}

		//Methods
		String GetErrorText() const;
		bool Parse(const FixedArray<String>& args);
		void PrintHelp();

		//Inline
		inline void AddHelpOption()
		{
			this->AddOption(this->helpOption);
		}

		inline bool IsHelpActivated()
		{
			return this->matchResult->IsActivated(this->helpOption);
		}

	private:
		//Members
		const String& programName;
		Option helpOption;
		UniquePointer<MatchResult> matchResult;

		//Methods
		bool HasHelpOption() const;
		const SubCommandArgument* HasSubCommandArgument() const;
		void PrintOptionHelp(const Option& option) const;
	};
}