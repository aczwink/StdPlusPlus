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

namespace StdXX
{
	namespace UI
	{
		class StyleContext
		{
		public:
			//Inline
			inline void AddPseudoClass(const String& pseudoClass)
			{
				this->pseudoClasses.Insert(pseudoClass);
			}

			inline void AddType(const String& typeName)
			{
				this->types.Insert(typeName);
			}

			inline bool HasPseudoClass(const String& pseudoClass) const
			{
				return this->pseudoClasses.Contains(pseudoClass);
			}

			inline bool HasPseudoClasses(const DynamicArray<String>& pseudoClasses) const
			{
				for (const String& pseudoClass : pseudoClasses)
				{
					if (!this->HasPseudoClass(pseudoClass))
						return false;
				}
				return true;
			}

			inline bool IsOfType(const String& typeName) const
			{
				return this->types.Contains(typeName);
			}

		private:
			//Members
			BinaryTreeSet<String> types;
			BinaryTreeSet<String> pseudoClasses;
		};
	}
}