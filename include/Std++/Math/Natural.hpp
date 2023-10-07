/*
 * Copyright (c) 2018-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Tuple.hpp>

namespace StdXX::Math
{
	/**
	 * Does include 0, such that a semiring is formed.
	 */
	class Natural
	{
		template<uint8 staticSegmentsCount>
		class NaturalStorage
		{
		public:
			//Constructor
			inline NaturalStorage() : lowValue(), nValidLowSegments(0)
			{
			}

			//Operators
			inline bool operator==(const NaturalStorage& rhs) const
			{
				return (this->nValidLowSegments == rhs.nValidLowSegments) and this->LowValuesEqual(rhs) and (this->highValue == rhs.highValue);
			}

			//Inline
			inline void AddSegment(uint64 segmentValue)
			{
				if( this->nValidLowSegments < staticSegmentsCount )
					this->lowValue[this->nValidLowSegments++] = segmentValue;
				else
					this->highValue.Push(segmentValue);
			}

			inline uint64 ClampTo64Bit() const
			{
				if(this->nValidLowSegments == 0)
					return 0;
				if(this->highValue.IsEmpty() and (this->nValidLowSegments == 1))
					return this->lowValue[0];
				return Unsigned<uint64>::Max();
			}

			inline void DropLeadingZeros()
			{
				while (!this->highValue.IsEmpty() and (this->highValue.Last() == 0))
					this->highValue.Pop();
				if(this->highValue.IsEmpty())
				{
					while( (this->nValidLowSegments > 0) and (this->lowValue[this->nValidLowSegments - 1] == 0) )
						this->nValidLowSegments--;
				}
			}

			inline uint32 GetNumberOfSegments() const
			{
				return this->highValue.GetNumberOfElements() + this->nValidLowSegments;
			}

			inline uint64 GetSegment(uint32 index) const
			{
				return (index < staticSegmentsCount) ? this->lowValue[index] : this->highValue[index - staticSegmentsCount];
			}

			inline void SetNumberOfSegments(uint32 count)
			{
				if(count <= staticSegmentsCount)
					this->nValidLowSegments = count;
				else
				{
					this->nValidLowSegments = staticSegmentsCount;
					this->highValue.Resize(count - staticSegmentsCount);
					for(uint32 i = 0; i < count - staticSegmentsCount; i++)
						this->highValue[i] = 0;
				}
			}

			inline void SetSegment(uint32 index, uint64 segmentValue)
			{
				if(index < staticSegmentsCount)
					this->lowValue[index] = segmentValue;
				else
					this->highValue[index - staticSegmentsCount] = segmentValue;
			}

		private:
			//Members
			DynamicArray<uint64> highValue;
			uint64 lowValue[staticSegmentsCount];
			uint8 nValidLowSegments;

			//Inline
			inline bool LowValuesEqual(const NaturalStorage& other) const
			{
				for(uint8 i = 0; i < this->nValidLowSegments; i++)
				{
					if(this->lowValue[i] != other.lowValue[i])
						return false;
				}
				return true;
			}
		};
	public:
		//Constructors
		/**
		 * Inits the value to zero.
		 */
		inline Natural() = default;
		inline Natural(const Natural& other) = default;
		inline Natural(Natural&& other) = default;
		Natural(const String& string);

		inline Natural(uint64 v)
		{
			if(v != 0)
				this->storage.AddSegment(v);
		}

		//Assignment operators
		Natural& operator=(const Natural& other) = default;
		Natural& operator=(Natural&& other) = default;

		//Arithmetic operators
		Natural operator+(const Natural& other) const;
		Natural operator-(const Natural& other) const;
		Natural operator*(const Natural& other) const;

		inline Natural operator/(const Natural& other) const
		{
			return this->DivMod(other).Get<0>();
		}

		inline Natural operator%(const Natural& other) const
		{
			return this->DivMod(other).Get<1>();
		}

		inline Natural& operator+=(const Natural& other)
		{
			*this = *this + other;
			return *this;
		}

		inline Natural& operator-=(const Natural& other)
		{
			*this = *this - other;
			return *this;
		}

		inline Natural& operator*=(const Natural& other)
		{
			*this = *this * other;
			return *this;
		}

		inline Natural& operator/=(const Natural& other)
		{
			*this = *this / other;
			return *this;
		}

		inline Natural& operator++() //pre-increment
		{
			*this += 1;
			return *this;
		}

		inline Natural& operator--() //pre-increment
		{
			*this -= 1;
			return *this;
		}

		//Binary operators
		Natural operator<<(uint64 shift) const;

		inline Natural& operator<<=(uint64 shift)
		{
			*this = (*this) << shift;
			return *this;
		}

		//Comparison operators
		bool operator<(const Natural& other) const;

		inline bool operator>(const Natural& other) const
		{
			return other < *this;
		}

		inline bool operator<=(const Natural& other) const
		{
			return !(*this > other);
		}

		inline bool operator==(const Natural& other) const
		{
			return this->storage == other.storage;
		}

		inline bool operator!=(const Natural& other) const
		{
			return !(this->storage == other.storage);
		}

		inline bool operator>=(const Natural& other) const
		{
			return other <= *this;
		}

		//Methods
		Tuple<Natural, Natural> DivMod(const Natural& divisor) const;
		String ToString() const;

		//Inline
		inline uint64 ClampTo64Bit() const
		{
			return this->storage.ClampTo64Bit();
		}

		inline bool IsEven() const
		{
			return (this->storage.GetSegment(0) & 1) == 0;
		}

	private:
		//Members
		NaturalStorage<1> storage;

		//Inline
		inline uint64 SafeRightShift(uint64 value, uint64 shiftAmount) const
		{
			//in C/C++ shifting 64 bit words is only defined for amounts between 0 and 63
			if(shiftAmount >= 64)
				return 0;
			return value >> shiftAmount;
		}
	};
}