/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include "String.hpp"

namespace StdXX
{
	class ConstStringIterator
	{
		friend class String;
	public:
		//Copy ctor
		inline ConstStringIterator(const ConstStringIterator& source)
			: string(source.string), current(source.current), position(source.position)
		{
		}

		//Operators
		inline ConstStringIterator &operator++() //Prefix ++
		{
			uint8 nBytes;

			this->string->Decode(this->current, nBytes);
			this->current += nBytes;
			this->position++;

			return *this;
		}

		inline ConstStringIterator &operator--() //Prefix--
		{
			if(this->string->IsUTF8())
			{
				do
				{
					this->current--;
				}
				while((*this->current & 0xC0) == 0x80);
			}
			else
			{
				this->current -= 2;
				if ( ((*(uint16 *)this->current) & 0xFFFFFC00) == 0xDC00)
					this->current -= 2; //surrogate pair
			}

			this->position--;

			return *this;
		}

		inline bool operator==(const ConstStringIterator &rhs) const
		{
			return this->string == rhs.string && this->position == rhs.position;
		}

		inline bool operator!=(const ConstStringIterator &rhs) const
		{
			return !(*this == rhs);
		}

		inline uint32 operator*() const
		{
			uint8 nBytes;

			return this->string->Decode(this->current, nBytes);
		}

		//Inline
		inline uint32 GetPosition() const
		{
			return this->position;
		}

	private:
		//Members
		const String* string;
		const byte *current;
		uint32 position; //In character units

		//Constructor
		inline ConstStringIterator(const String &string, const byte *start, uint32 pos) : string(&string), current(start), position(pos)
		{
		}

		//Inline
		inline bool Equals(ConstStringIterator other, uint32 length) const
		{
			ConstStringIterator copy = *this;
			while(length--)
			{
				if(*copy != *other)
					return false;
				++copy;
				++other;
			}

			return true;
		}

		inline bool EqualsReversed(ConstStringIterator other, uint32 length) const
		{
			ConstStringIterator copy = *this;
			while(length--)
			{
				if(*copy != *other)
					return false;
				--copy;
				--other;
			}

			return true;
		}

		inline uint32 GetByteOffset() const
		{
			return static_cast<uint32>(this->current - this->string->GetRawData());
		}
	};
}