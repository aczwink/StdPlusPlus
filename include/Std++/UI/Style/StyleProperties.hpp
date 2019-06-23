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
#include "StyleValue.hpp"

namespace StdXX
{
	namespace UI
	{
		class StyleProperties
		{
		public:
			//Properties
			inline const StdXX::Color& BackgroundColor() const
			{
				return this->properties[u8"background-color"].Color();
			}

			inline const StdXX::Color& Color() const
			{
				return this->properties[u8"color"].Color();
			}

			inline float64 PaddingBottom() const
			{
				return this->properties[u8"padding-bottom"].Number();
			}

			inline float64 PaddingTop() const
			{
				return this->properties[u8"padding-top"].Number();
			}

			//Inline
			inline void Set(const String& key, const StyleValue& value)
			{
				this->properties[key.ToLowercase()] = value;
			}

			inline void Set(const String& key, StyleValue&& value)
			{
				this->properties[key.ToLowercase()] = Move(value);
			}

			//For range-based loop
			inline auto begin() const
			{
				return this->properties.begin();
			}

			inline auto end() const
			{
				return this->properties.end();
			}

		private:
			//Members
			Map<String, StyleValue> properties;
		};
	}
}