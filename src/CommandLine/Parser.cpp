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
#include <Std++/CommandLine/Parser.hpp>
//Local
#include <Std++/Streams/Writers/StdOut.hpp>
#include <Std++/FileSystem/Path.hpp>
#include <Std++/CommandLine/SubCommandArgument.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommandLine;

//Public methods
String Parser::GetErrorText() const
{
	DynamicArray<String> lines;
	for(const auto& t : this->matchResult->ArgumentParseErrors())
	{
		switch(t.Get<0>())
		{
			case MatchResult::ArgumentParseError::MissingPositionalArgument:
				lines.Push(u8"Missing positional argument: " + t.Get<1>()->Name());
				break;
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
	ASSERT_EQUALS(this->matchResult->OptionParseErrors().IsEmpty(), true); //TODO: implement me

	if(!this->matchResult->ExtraParameters().IsEmpty())
		lines.Push(u8"The following parameters could not be recognized: '" + String::Join(this->matchResult->ExtraParameters(), u8"', '") + u8"'");

	if(this->HasHelpOption())
	{
		String shortName;
		shortName += this->helpOption.ShortName();
		lines.Push(u8"Use -" + shortName + u8" or --" + this->helpOption.LongName() + u8" to get help");
	}

	return String::Join(lines, u8".\n");
}

bool Parser::Parse(const FixedArray<String> &args)
{
	const SubCommandArgument* subCommandArgument;

	if( (args.GetNumberOfElements() == 1) and this->HasHelpOption() )
	{
		//help is always allowed if specified alone no matter what
		this->matchResult = new MatchResult(args);
		this->helpOption.Match(*this->matchResult);
		if(this->matchResult->IsValid() && this->matchResult->IsActivated(this->helpOption))
			return true;
	}
	else if( (args.GetNumberOfElements() == 2) and this->HasHelpOption() and (subCommandArgument = this->HasSubCommandArgument()) )
	{
		//help with subcommand alone is always allowed no matter what
		this->matchResult = new MatchResult(args);
		this->helpOption.Match(*this->matchResult);
		subCommandArgument->Match(*this->matchResult);
		if(this->matchResult->IsAtEnd() && this->matchResult->IsActivated(this->helpOption))
			return true;
	}

	this->matchResult = new MatchResult(args);
	this->Match(*this->matchResult);
	this->matchResult->Finished();
	return this->matchResult->IsValid();
}

void Parser::PrintHelp()
{
	const SubCommandArgument* subCommandArgument = this->HasSubCommandArgument();
	const Group* extraGroup = nullptr;

	FileSystem::Path programName = this->programName;
	stdOut << u8"Usage: " << programName.GetName() << u8" [Options]";
	for(const Argument* argument : this->positionalArgs)
	{
		if((argument == subCommandArgument) && this->matchResult->IsActivated(*subCommandArgument))
		{
			for(const Group*const& command : subCommandArgument->Commands())
			{
				if(this->matchResult->IsActivated(*command))
				{
					extraGroup = command;
					stdOut << u8" " << command->Name();
					for (const auto &t : extraGroup->PositionalArguments())
					{
						stdOut << u8" " << t->Name();
					}
					break;
				}
			}

			if(extraGroup)
				continue;
		}
		stdOut << u8" " << argument->Name();
	}
	stdOut << endl << endl;

	stdOut << u8"Arguments: " << endl;
	if(extraGroup)
	{
		for (const auto &t : extraGroup->PositionalArguments())
		{
			stdOut << u8"  " << t->ToString() << endl;
		}
	}
	for(const auto& t : this->positionalArgs)
	{
		if( (t == subCommandArgument) && extraGroup)
			continue;
		stdOut << u8"  " << t->ToString() << endl;
	}
	stdOut << endl;

	stdOut << u8"Options: " << endl;
	if(extraGroup)
	{
		for(const auto& kv : extraGroup->LongOptionsMap())
			this->PrintOptionHelp(*kv.value);
	}
	for(const auto& kv : this->longNameOptionsMap)
		this->PrintOptionHelp(*kv.value);
}

//Private methods
bool Parser::HasHelpOption() const
{
	return this->longNameOptionsMap.Contains(this->helpOption.LongName()) and (this->longNameOptionsMap[this->helpOption.LongName()] == &this->helpOption);
}

const SubCommandArgument *Parser::HasSubCommandArgument() const
{
	for(const Argument* argument : this->positionalArgs)
	{
		const SubCommandArgument *temp = dynamic_cast<const SubCommandArgument *>(argument);
		if(temp)
			return temp;
	}
	return nullptr;
}

void Parser::PrintOptionHelp(const Option &option) const
{
	stdOut << option.ToString() << endl
		   << u8"    " << option.Description() << endl
		   << endl;
}