/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include "StdXXCSSLexer.hpp"
//Local
#include <Std++/Char.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
Token StdXXCSSLexer::GetNextToken(bool ignoreWhitespace)
{
	while (!this->IsAtEnd())
	{
		//whites
		if (IsWhiteSpaceChar(*this->it))
		{
			++this->it;
			while (IsWhiteSpaceChar(*this->it))
				++this->it;

			if(ignoreWhitespace)
				continue;
			if(*this->it == u8'{')
				continue;
			return Token::Whitespace;
		}

		//special symbols
		switch (*this->it)
		{
		case u8':':
			++this->it;
			return Token::Colon;
		case u8';':
			++this->it;
			return Token::Semicolon;
		case u8'{':
			++this->it;
			return Token::LeftBrace;
		case u8'}':
			++this->it;
			return Token::RightBrace;
		}

		//string
		this->tokenValue = u8"";
		while (true)
		{
			if (IsWhiteSpaceChar(*this->it))
				break;

			bool ok;
			switch (*this->it)
			{
			case u8':':
			case u8';':
				ok = false;
				break;
			default:
				ok = true;
			}
			if (!ok)
				break;

			this->tokenValue += *this->it;
			++this->it;
		}
		return Token::String;
	}

	return Token::End;
}