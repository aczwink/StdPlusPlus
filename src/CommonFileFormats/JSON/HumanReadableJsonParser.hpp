/*
 * Copyright (c) 2019,2021 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/CommonFileFormats/JSON.hpp>

namespace _stdxx_
{
	class HumanReadableJsonParser
	{
	public:
		//Constructor
		inline HumanReadableJsonParser(StdXX::TextReader &textReader) : textReader(textReader)
		{
		}

		//Methods
		StdXX::CommonFileFormats::JsonValue Parse();

	private:
		//Members
		uint32 lookahead;
		StdXX::TextReader &textReader;

		//Methods
		StdXX::CommonFileFormats::JsonValue ParseNumber();
		StdXX::CommonFileFormats::JsonValue ParseObject();
		StdXX::CommonFileFormats::JsonValue ParseStringLiteral();
		StdXX::CommonFileFormats::JsonValue ParseTrue();
		StdXX::CommonFileFormats::JsonValue ParseValue();
		void SkipWhitespaces();

		//Inline
		inline bool Accept(uint32 codePoint)
		{
			if(this->lookahead == codePoint)
			{
				this->UpdateLookahead();
				return true;
			}
			return false;
		}

		inline void Expect(uint32 codePoint)
		{
			ASSERT(codePoint == this->lookahead, u8"Invalid code point.");
			this->UpdateLookahead();
		}

		inline void UpdateLookahead()
		{
			if(this->textReader.IsAtEnd())
				this->lookahead = 0;
			else
				this->lookahead = this->textReader.ReadCodePoint();
		}
	};
}