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
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++//Containers/Optional.hpp>

namespace StdXX
{
	template<typename EnumeratorType, typename EntryType>
	class EnumeratorIterator
	{
	public:
		//Constructors
		inline EnumeratorIterator() = default;

		inline EnumeratorIterator(UniquePointer<EnumeratorType>&& enumerator) : enumerator(Move(enumerator))
		{
			this->Advance();
		}

		inline EnumeratorIterator(EnumeratorIterator &&other) : enumerator(Move(other.enumerator))
		{
		}

		//Operators
		inline EnumeratorIterator &operator++() //Prefix++
		{
			this->Advance();
			return *this;
		}

		inline bool operator==(const EnumeratorIterator &other) const
		{
			return this->enumerator == other.enumerator;
		}

		inline bool operator!=(const EnumeratorIterator &other) const
		{
			return !(*this == other);
		}

		inline const EntryType& operator*() const
		{
			return this->enumerator->GetCurrent();
		}

	private:
		//Members
		UniquePointer<EnumeratorType> enumerator;

		//Inline
		virtual void Advance()
		{
			if(!this->enumerator->MoveForward())
				this->enumerator = nullptr;
		}
	};
}