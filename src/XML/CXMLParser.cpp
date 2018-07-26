/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
//Class Header
#include "CXMLParser.hpp"
//Local
#include <Std++/Debug.hpp>
#include <Std++/Memory.h>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX::Math;

//Constructor
CXMLParser::CXMLParser(InputStream &refInput) : input(refInput)
{
	this->pDoc = new Document;

	for (uint8 i = 0; i < sizeof(this->lookAhead) / sizeof(this->lookAhead[0]); i++)
		this->UpdateLookAhead();
}

//Destructor
CXMLParser::~CXMLParser()
{
	if (this->pDoc)
		delete this->pDoc;
}

//Private methods
Element *CXMLParser::ParseElement()
{
	bool readBody;
	Node *pChild;
	Element *pElement;
	String elementName;

	readBody = true;

	this->SkipWhitespaces();

	this->ExpectChar('<');
	elementName = this->ParseName();
	pElement = new Element(elementName);

	this->ReadAttributes(pElement->GetAttributes());
	if(this->AcceptChar('/'))
		readBody = false;
	this->ExpectChar('>');

	//read body
	if(readBody)
	{
		while(true)
		{
			this->SkipWhitespaces();

			if(this->lookAhead[0] == '<')
			{
				if(this->lookAhead[1] == '/')
				{
					//closing tag
					this->UpdateLookAhead();
					this->UpdateLookAhead();

					elementName = this->ParseName();

					ASSERT(elementName == pElement->GetName(), "If you see this, report to StdXX");
					this->ExpectChar('>');
					break;
				}
				else
				{
					//child tag
					pChild = this->ParseElement();
					pElement->AppendChild(pChild);
				}
			}
			else
			{
				//text
				pChild = this->ParseText();
				pElement->AppendChild(pChild);
			}
		}
	}

	return pElement;
}

String CXMLParser::ParseName()
{
	String name;

	//2.3

	static const auto &refNameStartCharFunc = [](uint32 c)
	{
		return (c == ':') ||
				IsValueInInterval(c, (uint32)'A', (uint32)'Z') ||
			   (c == '_') ||
				IsValueInInterval(c, (uint32)'a', (uint32)'z')
			;
	};

	static const auto &refNameCharFunc = [](uint32 c)
	{
		return (c == '-') ||
			   (c == '.') ||
				IsValueInInterval(c, (uint32)'0', (uint32)'9')
			;
	};

	ASSERT(refNameStartCharFunc(this->lookAhead[0]), "If you see this, report to StdXX");

	while (refNameCharFunc(this->lookAhead[0]) || refNameStartCharFunc(this->lookAhead[0]))
		name += this->ReadNextCodepoint();

	return name;
}

TextNode *CXMLParser::ParseText()
{
	OldString text;

	while (true)
	{
		switch (this->lookAhead[0])
		{
			case '<':
				goto end;
		}

		text += this->ReadNextCodepoint();
	}

	end:;
	return new TextNode(text);
}

void CXMLParser::ReadAttributes(Map<String, String> &refAttributes)
{
	String key, value;

	while (true)
	{
		this->SkipWhitespaces();

		switch (this->lookAhead[0])
		{
			case '>':
			case '/':
			case '?':
				goto end;
		}

		key = this->ParseName();
		this->ExpectChar('=');
		value = this->ReadAttributeValue();

		refAttributes.Insert(key, value);
	}
	end:;
}

String CXMLParser::ReadAttributeValue()
{
	String result;

	this->ExpectChar('"');

	while (true)
	{
		if (this->AcceptChar('"'))
			break;

		result += this->ReadNextCodepoint();
	}

	return result;
}

void CXMLParser::ReadProlog()
{
	String xmlName;
	Map<String, String> attributes;

	//2.8

	//start
	this->ExpectChar('<');
	this->ExpectChar('?');
	xmlName = this->ParseName();
	ASSERT(xmlName == "xml", "If you see this, report to StdXX");

	this->ReadAttributes(attributes);

	//version
	ASSERT(attributes.Contains("version"), "If you see this, report to StdXX");
	ASSERT(attributes["version"] == "1.0", "If you see this, report to StdXX");

	//encoding
	ASSERT(attributes.Contains("encoding"), "If you see this, report to StdXX");
	ASSERT(attributes["encoding"].ToLowercase() == "utf-8", "If you see this, report to StdXX");

	//end
	this->ExpectChar('?');
	this->ExpectChar('>');
}

void CXMLParser::SkipWhitespaces()
{
	restart:;
	//whitespaces
	while (this->IsWhitespace())
		this->UpdateLookAhead();

	//comment
	if (this->lookAhead[0] == '<' && this->lookAhead[1] == '!' && this->lookAhead[2] == '-' && this->lookAhead[3] == '-')
	{
		this->UpdateLookAhead();
		this->UpdateLookAhead();
		this->UpdateLookAhead();
		this->UpdateLookAhead();

		while (true)
		{
			if (this->lookAhead[0] == '-' && this->lookAhead[1] == '-' && this->lookAhead[2] == '>')
			{
				this->UpdateLookAhead();
				this->UpdateLookAhead();
				this->UpdateLookAhead();

				break;
			}

			this->UpdateLookAhead();
		}

		goto restart;
	}
}

//Public methods
Document *CXMLParser::Parse()
{
	Element *pRoot;

	this->ReadProlog();
	pRoot = this->ParseElement();

	this->pDoc->SetRootElement(pRoot);

	//unset doc pointer in object
	Document *pDoc;

	pDoc = this->pDoc;
	this->pDoc = nullptr;

	return pDoc;
}