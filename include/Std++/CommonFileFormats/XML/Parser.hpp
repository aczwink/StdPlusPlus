/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include <Std++/Streams/InputStream.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/Errorhandling/Exceptions/XMLParseException.hpp>

/*
XML Specification:
	https://www.w3.org/TR/REC-xml
*/
namespace StdXX::CommonFileFormats::XML
{
	class ParserCallbacks
	{
	public:
		//Abstract
		virtual void OnBeginElement(String&& elementName, BinaryTreeMap<String, String>&& attributes) = 0;
		virtual void OnEndElement(String&& elementName) = 0;
		virtual void OnText(String&& text) = 0;
	};

	class Parser
	{
	public:
		//Constructor
		Parser(InputStream& inputStream);

		//Methods
		void Parse(ParserCallbacks& callbacks);

	private:
		//Members
		uint32 lookaheads[4]; //we need four lookahead chars for comment begin "<!--"
		uint8 currentLookaheadIndex;
		InputStream& inputStream;
		UniquePointer<TextReader> textReader;

		//Methods
		void ParseBeginElement(ParserCallbacks& callbacks);
		void ParseCloseElement(ParserCallbacks& callbacks);
		void ParseComment();
		void ParseText(ParserCallbacks& callbacks);
		String ParseName();
		void ParseNext(ParserCallbacks& callbacks);
		BinaryTreeMap<String, String> ReadAttributes();
		String ReadAttributeValue();
		uint32 ReadEscapedChar();
		void ReadProlog();

		//Inline
		inline bool AcceptChar(uint32 c)
		{
			if (this->Lookahead(0) == c)
			{
				this->UpdateLookAhead();
				return true;
			}

			return false;
		}

		inline void ExpectChar(uint32 c)
		{
			if(this->Lookahead(0) != c)
				throw ErrorHandling::XMLParseException();
			this->UpdateLookAhead();
		}

		inline bool IsAtCloseTag() const
		{
			return (this->Lookahead(0) == u8'<') and (this->Lookahead(1) == u8'/');
		}

		inline bool IsAtComment() const
		{
			return (this->Lookahead(0) == u8'<') and (this->Lookahead(1) == u8'!') and
				(this->Lookahead(2) == u8'-') and (this->Lookahead(3) == u8'-');
		}

		inline bool IsAtOpenTag() const
		{
			return (this->Lookahead(0) == u8'<') and (this->Lookahead(1) != u8'!') and (this->Lookahead(1) != u8'/');
		}

		inline bool IsXMLWhiteSpaceChar(uint32 codePoint) const
		{
			switch(codePoint) //not all whitespace chars are allowed in xml 1.0
			{
				case u8'\t':
				case u8'\n':
				case u8'\r':
				case u8' ':
					return true;
			}
			return false;
		}

		inline uint32 Lookahead(uint8 next) const
		{
			return this->lookaheads[(this->currentLookaheadIndex + next) % 4];
		}

		inline uint32 ReadNextCodepoint()
		{
			uint32 next;

			next = this->Lookahead(0);
			this->UpdateLookAhead();

			return next;
		}

		inline void SkipWhitespaces()
		{
			while(IsXMLWhiteSpaceChar(this->Lookahead(0)))
				this->UpdateLookAhead();
		}

		inline void UpdateLookAhead()
		{
			if(this->textReader->IsAtEnd())
				this->lookaheads[this->currentLookaheadIndex] = 0;
			else
				this->lookaheads[this->currentLookaheadIndex] = this->textReader->ReadCodePoint();
			this->currentLookaheadIndex = (this->currentLookaheadIndex + 1) % 4;
		}
	};
}