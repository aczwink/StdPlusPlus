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
//Class header
#include <Std++/CommandLine/SubCommandArgument.hpp>
//Local
#include <Std++/CommandLine/MatchResult.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommandLine;

//Public methods
void SubCommandArgument::Match(MatchResult& matchResult) const
{
	if(matchResult.IsAtEnd())
	{
		matchResult.AddArgumentError(MatchResult::ArgumentParseError::MissingPositionalArgument, this);
		return;
	}

	for(const Group* const& command : this->commands)
	{
		MatchResult copy = matchResult;
		command->MatchOptions(copy);

		if(copy.Next() != command->Name())
			continue;

		command->MatchOptions(matchResult);

		matchResult.Advance();
		matchResult.Activate(this);
		matchResult.Activate(command);

		command->Match(matchResult);
		return;
	}

	matchResult.AddArgumentError(MatchResult::ArgumentParseError::MissingPositionalArgument, this);
}

String SubCommandArgument::ToString() const
{
	DynamicArray<String> names;
	for(const Group* const& command : this->commands)
		names.Push(command->Name() + u8"\t" + command->Description());
	String separator = u8"\n    ";
	return Argument::ToString() + u8"." + separator + u8"Choices:" + separator + String::Join(names, separator);
}
