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
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "StyleContext.hpp"

namespace StdXX
{
	namespace UI
	{
		//Forward declarations
		class Widget;

		enum class StyleCombinator
		{
			Descendant,
		};

		class StyleSelector
		{
		public:
			//Constructor
			inline StyleSelector() : type(u8"*")
			{
			}

			inline StyleSelector(String&& type, DynamicArray<String>&& pseudoClasses) : type(Move(type)), pseudoClasses(Move(pseudoClasses))
			{
			}

			StyleSelector(StyleSelector&&) = default;

			//Operators
			StyleSelector& operator=(StyleSelector&&) = default;

			//Methods
			bool Matches(const Widget& widget) const;
			bool Matches(const StyleContext& context, const Widget& parent) const;

			//Inline
			inline void CombineWith(StyleSelector&& other, StyleCombinator combinator)
			{
				this->other = new StyleSelector(Move(other));
				this->combinator = combinator;
			}

		private:
			//Members
			String type;
			DynamicArray<String> pseudoClasses;
			UniquePointer<StyleSelector> other;
			StyleCombinator combinator;

			//Methods
			bool Matches(const StyleContext& context) const;
		};
	}
}