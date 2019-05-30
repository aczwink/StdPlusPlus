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

StyleProperties StyleSheet::QueryVirtual(const StyleContext& context, const Widget& parent)
{
	StyleProperties result;
	for (const StyleRule& rule : this->rules)
	{
		if (rule.Matches(context, parent))
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
		while (true)
		{
			String tokenValue = lexer.GetTokenValue();
			if (lexer.Accept(Token::String))
				value += u8" " + tokenValue;
			else
				break;
		}
		lexer.Expect(Token::Semicolon);

		key = key.ToLowercase();
		if (key == u8"padding")
		{
			DynamicArray<String> parts = value.Split(u8" ");
			if (parts.GetNumberOfElements() == 2)
			{
				StyleValue vert = this->ParseValue(parts[0]);
				rule.Properties().Set(u8"padding-top", vert);
				rule.Properties().Set(u8"padding-bottom", vert);

				StyleValue horz = this->ParseValue(parts[1]);
				return;
			}
		}
		
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
	DynamicArray<StyleSelector> simpleSelectors;
	DynamicArray<StyleCombinator> combinators;

	bool atEnd = false;
	while (!atEnd)
	{
		StyleSelector inner = this->ParseSimpleSelector(lexer);
		simpleSelectors.Push(Move(inner));

		switch (lexer.GetCurrentToken())
		{
		case Token::LeftBrace:
			atEnd = true;
			break;
		case Token::Whitespace:
			combinators.Push(StyleCombinator::Descendant);
			lexer.ConsumeCurrentToken();
			break;
		}
	}

	for (uint32 i = 1; i < simpleSelectors.GetNumberOfElements(); i++)
		simpleSelectors[i].CombineWith(Move(simpleSelectors[i - 1]), combinators[i - 1]);
	return Move(simpleSelectors.Last());
}

StyleSelector StyleSheet::ParseSimpleSelector(_stdxx_::StdXXCSSLexer& lexer) const
{
	String type;
	DynamicArray<String> pseudoClasses;

	Token t = lexer.GetCurrentToken();
	bool atEnd = false;
	while (!atEnd)
	{
		String tokenValue = lexer.GetTokenValue();

		//check selector type
		switch (t)
		{
		case Token::Colon: //a pseudo class
		{
			lexer.ConsumeCurrentToken(false);
			ASSERT(lexer.GetCurrentToken() == Token::String, u8"TODO: DO THIS CORRECTLY");
			pseudoClasses.Push(lexer.GetTokenValue());
		}
		break;
		case Token::String: //a type selector
			type = tokenValue;
			break;
		case Token::LeftBrace:
		case Token::Whitespace:
			atEnd = true;
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
		}

		if (!atEnd)
		{
			lexer.ConsumeCurrentToken(false);
			t = lexer.GetCurrentToken();
		}
	}

	return { Move(type), Move(pseudoClasses) };
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

	//check for "rem" length
	if (value.EndsWith(u8"rem"))
	{
		return value.SubString(0, value.GetLength() - 3).ToFloat() * 16; //TODO: 16px default root element font size. this should be of course dependant on root
	}

	return value;
}