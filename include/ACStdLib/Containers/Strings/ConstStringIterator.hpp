/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Local
#include "String.hpp"

namespace ACStdLib
{
	class ConstStringIterator
	{
		friend class String;
	public:
		//Operators
		inline ConstStringIterator &operator++() //Prefix ++
		{
			uint8 nBytes;

			this->string.Decode(this->current, nBytes);
			this->current += nBytes;
			this->position++;

			return *this;
		}

		inline ConstStringIterator &operator--() //Prefix--
		{
			if(this->string.IsUTF8())
			{
				do
				{
					this->current--;
				}
				while((*this->current & 0xC0) == 0x80);
			}
			else
				NOT_IMPLEMENTED_ERROR; //TODO: impelment me

			this->position--;

			return *this;
		}

		inline uint32 operator*() const
		{
			uint8 nBytes;

			return this->string.Decode(this->current, nBytes);
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

		inline uint32 GetPosition() const
		{
			return this->position;
		}

	private:
		//Members
		const String &string;
		const byte *current;
		uint32 position; //In character units

		//Constructor
		inline ConstStringIterator(const String &string, const byte *start, uint32 pos) : string(string), current(start), position(pos)
		{
		}
	};
}