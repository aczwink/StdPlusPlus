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
#include <Std++/CommonFileFormats/XML/Parser.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats::XML;

//Constructor
Parser::Parser(InputStream &inputStream) : inputStream(inputStream)
{
	this->currentLookaheadIndex = 0;
}

//Public methods
void Parser::Parse(ParserCallbacks& callbacks)
{
	this->ReadProlog();

	while(!this->inputStream.IsAtEnd())
	{
		this->ParseNext(callbacks);
	}
}

//Private methods
void Parser::ParseBeginElement(ParserCallbacks& callbacks)
{
	this->ExpectChar(u8'<');
	String elementName = this->ParseName();

	Map<String, String> attributes = this->ReadAttributes();

	bool empty = this->AcceptChar(u8'/');
	this->ExpectChar(u8'>');

	String elementBackup = elementName;
	callbacks.OnBeginElement(Move(elementName), Move(attributes));

	if(empty)
		callbacks.OnEndElement(Move(elementBackup));
}

void Parser::ParseCloseElement(ParserCallbacks& callbacks)
{
	this->ExpectChar(u8'<');
	this->ExpectChar(u8'/');
	String elementName = this->ParseName();
	this->ExpectChar(u8'>');

	callbacks.OnEndElement(Move(elementName));
}

void Parser::ParseComment()
{
	for(uint8 i = 0; i < 4; i++)
		this->UpdateLookAhead();

	while (true)
	{
		if (this->Lookahead(0) == u8'-' && this->Lookahead(1) == u8'-' && this->Lookahead(2) == u8'>')
		{
			this->UpdateLookAhead();
			this->UpdateLookAhead();
			this->UpdateLookAhead();

			break;
		}

		this->UpdateLookAhead();
	}
}

void Parser::ParseText(ParserCallbacks& callbacks)
{
	String text;
	while (this->Lookahead(0) != u8'<')
	{
		if(this->Lookahead(0) == u8'&')
			text += this->ReadEscapedChar();
		else
			text += this->ReadNextCodepoint();
	}

	callbacks.OnText(Move(text));
}

String Parser::ParseName()
{
	String name;

	//2.3

	static const auto &nameStartCharFunc = [](uint32 c)
	{
		return (c == ':') ||
			   Math::IsValueInInterval(c, (uint32)'A', (uint32)'Z') ||
			   (c == '_') ||
			   Math::IsValueInInterval(c, (uint32)'a', (uint32)'z')
				;
	};

	static const auto &nameCharFunc = [](uint32 c)
	{
		return (c == '-') ||
			   (c == '.') ||
			   Math::IsValueInInterval(c, (uint32)'0', (uint32)'9')
				;
	};

	ASSERT(nameStartCharFunc(this->Lookahead(0)), "If you see this, report to StdXX");

	while (nameCharFunc(this->Lookahead(0)) || nameStartCharFunc(this->Lookahead(0)))
		name += this->ReadNextCodepoint();

	return name;
}

void Parser::ParseNext(ParserCallbacks& callbacks)
{
	this->SkipWhitespaces();

	if(this->IsAtOpenTag())
		this->ParseBeginElement(callbacks);
	else if(this->IsAtCloseTag())
		this->ParseCloseElement(callbacks);
	else if(this->IsAtComment())
		this->ParseComment();
	else
		this->ParseText(callbacks);
}

Map<String, String> Parser::ReadAttributes()
{
	Map<String, String> attributes;
	while (true)
	{
		this->SkipWhitespaces();

		switch (this->Lookahead(0))
		{
			case u8'>':
			case u8'/':
			case u8'?':
				goto end;
		}

		String key = this->ParseName();
		this->ExpectChar('=');
		String value = this->ReadAttributeValue();

		attributes.Insert(key, value);
	}
	end:;
	return attributes;
}

String Parser::ReadAttributeValue()
{
	String result;

	this->ExpectChar(u8'"');

	while (true)
	{
		if (this->AcceptChar(u8'"'))
			break;
		if(this->Lookahead(0) == u8'&')
			result += this->ReadEscapedChar();
		else
			result += this->ReadNextCodepoint();
	}

	return result;
}

uint32 Parser::ReadEscapedChar()
{
	this->ExpectChar(u8'&');
	String chars;
	for(uint8 i = 0; (i < 4) and (this->Lookahead(0) != u8';'); i++)
		chars += this->ReadNextCodepoint();
	this->ExpectChar(u8';');

	if(chars == u8"quot")
		return u8'"';
	if(chars == u8"apos")
		return u8'\'';
	if(chars == u8"lt")
		return u8'<';
	if(chars == u8"gt")
		return u8'>';
	if(chars == u8"amp")
		return u8'&';

	throw ErrorHandling::XMLParseException();
}

void Parser::ReadProlog()
{
	//2.8

	this->textReader = new TextReader(this->inputStream, TextCodecType::UTF8); //assume utf-8 for header

	for (uint8 i = 0; i < sizeof(this->lookaheads) / sizeof(this->lookaheads[0]); i++)
		this->UpdateLookAhead();

	//start
	this->ExpectChar(u8'<');
	this->ExpectChar(u8'?');
	String xmlName = this->ParseName();
	ASSERT(xmlName == u8"xml", "If you see this, report to StdXX");

	Map<String, String> attributes = this->ReadAttributes();

	//version
	ASSERT(attributes.Contains("version"), "If you see this, report to StdXX");
	ASSERT(attributes["version"] == "1.0", "If you see this, report to StdXX");

	//encoding
	ASSERT(attributes.Contains("encoding"), "If you see this, report to StdXX");
	ASSERT(attributes["encoding"].ToLowercase() == "utf-8", "If you see this, report to StdXX");
	this->textReader = new TextReader(this->inputStream, TextCodecType::UTF8);

	//end
	this->ExpectChar('?');
	this->ExpectChar('>');
}