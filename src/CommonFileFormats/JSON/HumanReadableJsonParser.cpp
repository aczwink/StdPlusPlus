/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
#include <StdXX.hpp>
#include "HumanReadableJsonParser.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::CommonFileFormats;

//Public methods
JsonValue HumanReadableJsonParser::Parse()
{
	this->UpdateLookahead();
	return this->ParseValue();
}

//Private methods
JsonValue HumanReadableJsonParser::ParseNumber()
{
	//at least one digit must exist
	ASSERT(Math::IsValueInInterval(this->lookahead, (uint32)u8'0', (uint32)u8'9'), u8"REPORT THIS PLEASE!");

	String tmp;
	while(Math::IsValueInInterval(this->lookahead, (uint32)u8'0', (uint32)u8'9'))
	{
		tmp += this->lookahead;
		this->UpdateLookahead();
	}

	return tmp.ToUInt();
}

JsonValue HumanReadableJsonParser::ParseObject()
{
	this->Expect(u8'{');

	JsonValue object = JsonValue::Object();

	this->SkipWhitespaces();
	if(this->Accept(u8'}')) //check for empty object
		return object;

	while(true)
	{
		JsonValue key = this->ParseStringLiteral();
		this->SkipWhitespaces();
		this->Expect(u8':');
		JsonValue value = this->ParseValue();

		object[key.StringValue()] = value;

		this->SkipWhitespaces();
		if(this->Accept(u8'}'))
			break;
		this->Expect(u8',');
		this->SkipWhitespaces();
		if(this->Accept(u8'}'))
		break;
	}
	return object;
}

JsonValue HumanReadableJsonParser::ParseStringLiteral()
{
	this->SkipWhitespaces();
	this->Expect(u8'"');

	String tmp;
	while(true)
	{
		if(this->Accept(u8'"'))
			break;
		tmp += this->lookahead;
		this->UpdateLookahead();
	}
	return tmp;
}

JsonValue HumanReadableJsonParser::ParseTrue()
{
	this->Expect(u8't');
	this->Expect(u8'r');
	this->Expect(u8'u');
	this->Expect(u8'e');

	return true;
}

JsonValue HumanReadableJsonParser::ParseValue()
{
	this->SkipWhitespaces();

	if(Math::IsValueInInterval(this->lookahead, (uint32)u8'0', (uint32)u8'9'))
		return this->ParseNumber();

	switch(this->lookahead)
	{
		case u8't':
			return this->ParseTrue();
		case u8'{':
			return this->ParseObject();
		case u8'"':
			return this->ParseStringLiteral();
	}

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return JsonValue();
}

void HumanReadableJsonParser::SkipWhitespaces()
{
	while(IsWhiteSpaceChar(this->lookahead))
	{
		if(this->textReader.IsAtEnd())
			break;
		this->lookahead = this->textReader.ReadCodePoint();
	}

	if(this->Accept(u8'/'))
	{
		this->Expect(u8'/');
		this->textReader.ReadLine();
		this->UpdateLookahead();

		this->SkipWhitespaces();
	}
}