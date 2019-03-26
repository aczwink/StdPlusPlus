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
#pragma once
//Local
#include "StyleRule.hpp"

//Forward declarations
namespace _stdxx_
{
	class StdXXCSSLexer;
}

namespace StdXX
{
	namespace UI
	{
		class StyleSheet
		{
		public:
			//Constructors
			StyleSheet() = default;

			inline StyleSheet(const String& string)
			{
				this->Parse(string);
			}

			//Methods
			void Parse(const String& string);
			StyleProperties Query(const Widget& widget);

			//Inline
			inline void SetVariable(const String& name, const StyleValue& value)
			{
				this->variables[name] = value;
			}

			//Functions
			inline static StyleSheet& Global()
			{
				static StyleSheet global;
				return global;
			}

		private:
			//Members
			DynamicArray<StyleRule> rules;
			Map<String, StyleValue> variables;

			//Methods
			void ParseProperties(StyleRule& rule, _stdxx_::StdXXCSSLexer& lexer) const;
			StyleRule ParseRule(_stdxx_::StdXXCSSLexer& lexer) const;
			StyleSelector ParseSelector(_stdxx_::StdXXCSSLexer& lexer) const;
			StyleValue ParseValue(const String& value) const;
		};
	}
}