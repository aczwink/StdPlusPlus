/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Tuple.hpp>

namespace StdXX::FileSystem
{
	class DirectoryIterator
	{
	public:
		//Constructors
		inline DirectoryIterator() = default;

		inline DirectoryIterator(UniquePointer<DirectoryEnumerator>&& enumerator) : enumerator(Move(enumerator))
		{
			this->Advance();
		}

		inline DirectoryIterator(DirectoryIterator &&other) : enumerator(Move(enumerator))
		{
		}

		//Operators
		inline DirectoryIterator &operator++() //Prefix++
		{
			this->Advance();
			return *this;
		}

		inline bool operator==(const DirectoryIterator &other) const
		{
			return this->enumerator.IsNull() && other.enumerator.IsNull();
		}

		inline bool operator!=(const DirectoryIterator &other) const
		{
			return !(*this == other);
		}

		inline const DirectoryEntry& operator*() const
		{
			return this->entry;
		}

	private:
		//Members
		UniquePointer<DirectoryEnumerator> enumerator;
		DirectoryEntry entry;

		//Inline
		inline void Advance()
		{
			if(!this->enumerator->Next(this->entry))
			{
				this->enumerator = nullptr;
				this->entry = {};
			}
			else if((this->entry.name == u8".") || (this->entry.name == u8".."))
				this->Advance();
		}
	};
}