/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
	template<typename T>
	class Bitfield
	{
	public:
		//Members
		T value;

		//Constructors
		inline Bitfield() : value()
		{
		}

		inline Bitfield(T value) : value(value)
		{
		}

		//Operators
		bool operator==(const Bitfield&) const = default;

		//Inline
		inline T Extract(uint8 bitPos, uint8 nBits) const
		{
			return this->Get(bitPos, nBits) << bitPos;
		}

		inline T Get(uint8 bitPos, uint8 nBits) const
		{
			ASSERT(bitPos < (sizeof(T) * 8), u8"Bit position is out of range");

			return (this->value >> bitPos) & ((1 << nBits) - 1);
		}

		inline bool IsBitSet(uint8 bitPos) const
		{
			return this->Get(bitPos, 1) != 0;
		}

		inline void Set(uint8 bitPos, uint8 nBits, T newValue)
		{
			newValue <<= bitPos;

			T mask = ((1 << nBits) - 1) << bitPos;
			ASSERT_EQUALS(newValue, newValue & mask); //check for overflow

			this->value = (this->value & ~mask) | newValue;
		}

		inline void SetBit(uint8 bitPos, bool newValue)
		{
			this->Set(bitPos, 1, newValue);
		}
	};
}