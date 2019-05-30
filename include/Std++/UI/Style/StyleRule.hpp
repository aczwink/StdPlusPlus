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
#include <Std++/Containers/Map/Map.hpp>
#include "StyleProperties.hpp"
#include "StyleSelector.hpp"

namespace StdXX
{
	namespace UI
	{
		class StyleRule
		{
		public:
			//Constructor
			inline StyleRule()
			{
			}

			inline StyleRule(StyleSelector&& selector) : selector(Move(selector))
			{
			}

			//Properties
			inline StyleProperties& Properties()
			{
				return this->properties;
			}

			inline const StyleProperties& Properties() const
			{
				return this->properties;
			}

			//Inline
			inline bool Matches(const Widget& widget) const
			{
				return this->selector.Matches(widget);
			}

			inline bool Matches(const StyleContext& styleContext, const Widget& parent) const
			{
				return this->selector.Matches(styleContext, parent);
			}

		private:
			//Members
			StyleSelector selector;
			StyleProperties properties;
		};
	}
}