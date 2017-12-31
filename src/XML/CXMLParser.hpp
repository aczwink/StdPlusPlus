/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include <ACStdLib/Streams/BufferedInputStream.hpp>
#include <ACStdLib/Streams/Readers/TextReader.hpp>
#include <ACStdLib/XML/Document.hpp>
#include <ACStdLib/XML/TextNode.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::XML;

/*
XML Specification:
	https://www.w3.org/TR/REC-xml
*/

class CXMLParser
{
public:
	//Constructor
	CXMLParser(InputStream &refInput);

	//Destructor
	~CXMLParser();

	//Methods
	Document *Parse();

private:
	//Members
	uint32 lookAhead[4]; //we need four lookahead chars for comment begin "<!--"
	Document *pDoc;
	BufferedInputStream input;

	//Methods
	Element *ParseElement();
	OldString ParseName();
	TextNode *ParseText();
	void ReadAttributes(Map<OldString, OldString> &refAttributes);
	OldString ReadAttributeValue();
	void ReadProlog();
	void SkipWhitespaces();

	//Inline
	inline bool AcceptChar(uint32 c)
	{
		if (this->lookAhead[0] == c)
		{
			this->UpdateLookAhead();
			return true;
		}

		return false;
	}

	inline void ExpectChar(uint32 c)
	{
		ASSERT(this->lookAhead[0] == c);
		this->UpdateLookAhead();
	}

	inline bool IsWhitespace()
	{
		switch (this->lookAhead[0])
		{
			case ' ':
			case '\r':
			case '\n':
			case '\t':
				return true;
		}
		return false;
	}

	inline uint32 ReadNextCodepoint()
	{
		uint32 next;

		next = this->lookAhead[0];
		this->UpdateLookAhead();

		return next;
	}

	inline void UpdateLookAhead()
	{
		for (uint8 i = 0; i < sizeof(this->lookAhead) / sizeof(this->lookAhead[0]) - 1; i++)
			this->lookAhead[i] = this->lookAhead[i + 1];

		this->lookAhead[sizeof(this->lookAhead) / sizeof(this->lookAhead[0]) - 1] = TextReader(this->input).ReadUTF8();
	}
};