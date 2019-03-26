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
#include <Std++/UI/Style/StyleSheet.hpp>
//Local
#include <Std++/UI/WidgetContainer.hpp>
#include <Std++/Char.hpp>
#include "StdXXCSSLexer.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Public methods
void StyleSheet::Parse(const String& string)
{
	StdXXCSSLexer lexer(string.begin(), string.end());
	while (!lexer.IsAtEnd())
	{
		this->rules.Push(this->ParseRule(lexer));
	}
}

StyleProperties StyleSheet::Query(const Widget& widget)
{
	StyleProperties result;
	for (const StyleRule& rule : this->rules)
	{
		if (rule.Matches(widget))
		{
			for (const auto& kv : rule.Properties())
			{
				result.Set(kv.key, kv.value);
			}
		}
	}

	return result;
}

//Private methods
void StyleSheet::ParseProperties(StyleRule& rule, StdXXCSSLexer& lexer) const
{
	while (lexer.GetCurrentToken() != Token::RightBrace)
	{
		String key = lexer.Expect(Token::String);
		lexer.Expect(Token::Colon);
		String value = lexer.Expect(Token::String);
		lexer.Expect(Token::Semicolon);

		rule.Properties().Set(Move(key), this->ParseValue(value));
	}
}

StyleRule StyleSheet::ParseRule(StdXXCSSLexer& lexer) const
{
	StyleSelector selector = this->ParseSelector(lexer);
	lexer.Expect(Token::LeftBrace);
	StyleRule rule(Move(selector));
	this->ParseProperties(rule, lexer);
	lexer.Expect(Token::RightBrace);

	return rule;
}

StyleSelector StyleSheet::ParseSelector(StdXXCSSLexer& lexer) const
{
	//check selector type
	String tokenValue = lexer.GetTokenValue();
	Token t = lexer.GetCurrentToken();
	lexer.ConsumeCurrentToken();

	//check for end
	if (lexer.GetCurrentToken() == Token::LeftBrace)
	{
		switch (t)
		{
		case Token::String: //a type selector
			return StyleSelector::Type(tokenValue);
		}
	}

	NOT_IMPLEMENTED_ERROR;
}

StyleValue StyleSheet::ParseValue(const String& value) const
{
	//check for var name
	if (value.StartsWith(u8"$"))
		return this->variables[value.SubString(1)];
	
	//check for hex color
	if ((value.GetLength() == 7) && value.StartsWith(u8"#"))
	{
		auto it = value.begin();
		++it;

		bool ok = true;
		Color c;
		c.a = 1;
		for (uint8 i = 0; i < 3; i++)
		{
			uint8 component = 0;
			for (uint8 j = 0; j < 2; j++)
			{
				component <<= 4;
				uint32 codePoint = StdXX::ToLowercase(*it);
				if (Math::IsValueInInterval(codePoint, uint32(u8'0'), uint32(u8'9')))
					component |= (codePoint - u8'0');
				else if (Math::IsValueInInterval(codePoint, uint32(u8'a'), uint32(u8'f')))
					component |= 10 + (codePoint - u8'a');
				else
					ok = false;
				++it;
			}

			float32 componentFloat = component / 255.0f;
			switch (i)
			{
			case 0:
				c.r = componentFloat;
				break;
			case 1:
				c.g = componentFloat;
				break;
			case 2:
				c.b = componentFloat;
				break;
			}
		}

		if (ok)
			return c;
	}

	return value;
}