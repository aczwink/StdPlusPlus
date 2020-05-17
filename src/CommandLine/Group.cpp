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
#include <Std++/CommandLine/Group.hpp>
//Local
#include <Std++/CommandLine/MatchResult.hpp>
#include <Std++/Containers/Strings/ConstStringIterator.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommandLine;

//Public methods
void Group::Match(MatchResult &result) const
{
	this->MatchOptions(result);
	for(const Argument* argument : this->positionalArgs)
	{
		argument->Match(result);
	}
}

void Group::MatchOptions(MatchResult &result) const
{
	while(!result.IsAtEnd())
	{
		const String& next = result.Next();
		const Option* option = nullptr;
		if(next == u8"--")
			break;
		else if(next.StartsWith(u8"--"))
		{
			const String& optionName = next.SubString(2);
			if(this->longNameOptionsMap.Contains(optionName))
				option = this->longNameOptionsMap[optionName];
		}
		else if(next.StartsWith(u8"-"))
		{
			ASSERT_EQUALS(2_u32, next.GetLength());

			const String& optionName = next.SubString(1);
			uint32 codePoint = *optionName.begin();
			if(this->shortNameOptionsMap.Contains(codePoint))
				option = this->shortNameOptionsMap[codePoint];
		}

		if(option)
			option->Match(result);
		else
			break;
	}
}
