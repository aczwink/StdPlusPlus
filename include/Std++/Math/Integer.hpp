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
#include "Natural.hpp"

namespace StdXX::Math
{
	class Integer
	{
	public:
		//Members
		bool isNegative;
		Natural absValue;

		//Constructors
		inline Integer() : isNegative(false)
		{
		}

		inline Integer(int64 value) : isNegative(value < 0), absValue(uint64((value < 0) ? -value : value))
		{
		}

		inline Integer(const Natural& absValue) : isNegative(false), absValue(absValue)
		{
		}

		inline Integer(Natural&& absValue) : isNegative(false), absValue(Forward<Natural>(absValue))
		{
		}

		inline Integer(bool isNegative, Natural&& absValue) : isNegative(isNegative), absValue(Forward<Natural>(absValue))
		{
		}

		inline Integer(const String& string) : isNegative(string.StartsWith(u8"-")), absValue(string.StartsWith(u8"+") or string.StartsWith(u8"-") ? string.SubString(1) : string)
		{
		}

		//Arithmetic operators
		inline Integer operator+(const Integer& rhs) const
		{
			const Integer& lhs = *this;
			if(lhs.isNegative == rhs.isNegative)
				return Integer(lhs.isNegative, lhs.absValue + rhs.absValue);

			bool less = lhs.absValue < rhs.absValue;
			bool resultIsNegative = lhs.isNegative == !less;
			return {resultIsNegative, less ? rhs.absValue - lhs.absValue : lhs.absValue - rhs.absValue};
		}

		inline Integer operator-(const Integer& rhs) const
		{
			if (this->isNegative && !rhs.isNegative)
				return Integer(true, this->absValue + rhs.absValue);
			if (!this->isNegative && rhs.isNegative)
				return Integer(false, this->absValue + rhs.absValue);
			if (this->absValue < rhs.absValue)
			{
				if (this->isNegative)
					return Integer(false, rhs.absValue - this->absValue);
				return Integer(true, rhs.absValue - this->absValue);
			}
			if (this->isNegative)
				return Integer(true, this->absValue - rhs.absValue);
			return Integer(false, this->absValue - rhs.absValue);
		}

		inline Integer operator*(const Integer& other) const
		{
			return Integer(this->isNegative != other.isNegative, this->absValue * other.absValue);
		}

		inline Integer operator/(const Integer& rhs) const
		{
			return Integer(this->isNegative != rhs.isNegative, this->absValue / rhs.absValue);
		}

		inline Integer& operator+=(const Integer& other)
		{
			*this = *this + other;
			return *this;
		}

		inline Integer& operator-=(const Integer& rhs)
		{
			*this = *this - rhs;
			return *this;
		}

		inline Integer& operator*=(const Integer& other)
		{
			*this = *this * other;
			return *this;
		}

		inline Integer& operator/=(const Integer& rhs)
		{
			*this = *this / rhs;
			return *this;
		}

		inline Integer& operator++() //pre-increment
		{
			*this += 1;
			return *this;
		}

		inline Integer& operator--() //pre-decrement
		{
			*this -= 1;
			return *this;
		}

		//Comparison operators
		inline bool operator<(const Integer& other) const
		{
			if (this->isNegative && !other.isNegative)
				return true;
			if (other.isNegative && !this->isNegative)
				return false;
			return this->absValue < other.absValue;
		}

		inline bool operator>(const Integer& rhs) const
		{
			return rhs.operator<(*this);
		}

		inline bool operator<=(const Integer& other) const
		{
			if (this->isNegative && !other.isNegative)
				return true;
			if (other.isNegative && !this->isNegative)
				return false;
			return this->absValue <= other.absValue;
		}

		inline bool operator>=(const Integer& other) const
		{
			return other <= *this;
		}

		inline bool operator==(const Integer& rhs) const
		{
			if(this->absValue == Natural())
				return rhs.absValue == this->absValue; //for 0 the sign doesn't matter
			return (this->isNegative == rhs.isNegative) and (this->absValue == rhs.absValue);
		}

		inline bool operator!=(const Integer& rhs) const
		{
			return !(*this == rhs);
		}

		//Inline
		inline int64 ClampTo64Bit() const
		{
			if(this->absValue > Signed<int64>::Max())
				return Signed<int64>::Max();
			if(this->isNegative and (this->absValue > ((uint64)Signed<int64>::Max() + 1)) )
				return Signed<int64>::Min();
			return this->isNegative ? -(int64)this->absValue.ClampTo64Bit() : this->absValue.ClampTo64Bit();
		}

		inline String ToString() const
		{
			if (this->isNegative)
				return u8"-" + this->absValue.ToString();
			return this->absValue.ToString();
		}
	};
}