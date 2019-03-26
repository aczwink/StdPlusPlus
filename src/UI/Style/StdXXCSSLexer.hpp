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
//Local
#include <Std++/Containers/Strings/ConstStringIterator.hpp>
#include <Std++/Containers/Strings/String.hpp>

namespace _stdxx_
{
	enum class Token
	{
		End,
		Colon,
		LeftBrace,
		RightBrace,
		Semicolon,
		String
	};

	class StdXXCSSLexer
	{
	public:
		//Constructor
		inline StdXXCSSLexer(const StdXX::ConstStringIterator& it, const StdXX::ConstStringIterator& end) : it(it), end(end)
		{
			this->currentToken = this->GetNextToken();
		}

		//Methods
		Token GetNextToken();

		//Inline
		inline bool Accept(Token t)
		{
			if (this->currentToken == t)
			{
				this->ConsumeCurrentToken();
				return true;
			}
			return false;
		}

		inline bool Accept(Token t, StdXX::String& tokenValue)
		{
			if (this->currentToken == t)
			{
				tokenValue = this->tokenValue;
				this->ConsumeCurrentToken();
				return true;
			}
			return false;
		}

		inline void ConsumeCurrentToken()
		{
			this->currentToken = this->GetNextToken();
		}

		inline StdXX::String Expect(Token t)
		{
			StdXX::String tokenValue = this->tokenValue;
			ASSERT(this->currentToken == t, u8"TODO: IMPLEMENT THIS CORRECTLY!");
			this->ConsumeCurrentToken();

			return tokenValue;
		}

		inline Token GetCurrentToken() const
		{
			return this->currentToken;
		}

		inline bool IsAtEnd() const
		{
			return this->it == this->end;
		}

		inline const StdXX::String& GetTokenValue() const
		{
			return this->tokenValue;
		}

	private:
		//Members
		StdXX::ConstStringIterator it;
		StdXX::ConstStringIterator end;
		Token currentToken;
		StdXX::String tokenValue;
	};
}