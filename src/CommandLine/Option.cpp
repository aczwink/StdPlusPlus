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
#include <Std++/CommandLine/Option.hpp>
//Local
#include <Std++/CommandLine/MatchResult.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommandLine;

//Public methods
void Option::Match(MatchResult &result) const
{
	if(this->MatchesName(result))
	{
		result.Advance();
		result.Activate(this);
	}
}

String Option::ToString() const
{
	String shortName;
	shortName += this->ShortName();
	return u8"  -" + shortName + u8", --" + this->LongName();
}

//Protected methods
bool Option::MatchesName(MatchResult &result) const
{
	if(result.IsAtEnd())
		return false;

	const String& next = result.Next();
	if(next == u8"--")
		return false;

	if(next.StartsWith(u8"--"))
		return next.SubString(2) == this->longName;
	else if(next.StartsWith(u8"-"))
	{
		String shortName;
		shortName += this->shortName;
		return  next.SubString(1, 1) == shortName;
	}

	return false;
}
