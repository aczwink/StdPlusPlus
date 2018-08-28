/*
* Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Array/DynamicArray.hpp>

namespace StdXX
{
	namespace Multimedia
	{
		template <typename EntryType>
		class Index
		{
		public:
			//Constructor
			inline Index() : entriesSorted(true)
			{
			}

			//Inline
			inline void AddEntry(const EntryType &entry)
			{
				if (this->entries.IsEmpty())
					this->entriesSorted = true;
				else
					this->entriesSorted = this->entriesSorted && this->entries[this->entries.GetNumberOfElements() - 1] <= entry;
				this->entries.Push(entry);
			}

			inline const DynamicArray<EntryType> &GetEntries() const
			{
				return this->entries;
			}

			inline const EntryType &GetEntry(uint32 index) const
			{
				return this->entries[index];
			}

			inline uint32 GetNumberOfEntries() const
			{
				return this->entries.GetNumberOfElements();
			}

		protected:
			//Inline
			inline void EnsureSorted()
			{
				if (!this->entriesSorted)
				{
					this->entries.Sort();
					this->entriesSorted = true;
				}
			}

		private:
			//Members
			bool entriesSorted;
			DynamicArray<EntryType> entries;
		};
	}
}