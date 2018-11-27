/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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

namespace StdXX
{
	namespace Math
	{
		class Integer
		{
		public:
			//Constructors
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

			//Arithmetic operators
			inline Integer operator+(const Integer& other) const
			{
				if (this->isNegative && !other.isNegative)
					return other - *this;
				if (!this->isNegative && other.isNegative)
					return *this - other;
				return Integer(this->isNegative, this->absValue + other.absValue);
			}

			inline Integer operator-(const Integer& other) const
			{
				if (this->isNegative && !other.isNegative)
					return Integer(true, this->absValue + other.absValue);
				if (!this->isNegative && other.isNegative)
					return Integer(false, this->absValue + other.absValue);
				if (this->absValue < other.absValue)
				{
					if (this->isNegative)
						return Integer(false, other.absValue - this->absValue);
					return Integer(true, other.absValue - this->absValue);
				}
				if (this->isNegative)
					return Integer(true, this->absValue - other.absValue);
				return Integer(false, this->absValue - other.absValue);
			}

			inline Integer operator*(const Integer& other) const
			{
				return Integer(this->isNegative != other.isNegative, this->absValue * other.absValue);
			}

			inline Integer& operator+=(const Integer& other)
			{
				*this = *this + other;
				return *this;
			}

			inline Integer& operator*=(const Integer& other)
			{
				*this = *this * other;
				return *this;
			}

			inline Integer& operator++() //pre-increment
			{
				*this += 1;
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

			//Inline
			inline String ToString() const
			{
				if (this->isNegative)
					return u8"-" + this->absValue.ToString();
				return this->absValue.ToString();
			}

		private:
			//Members
			bool isNegative;
			Natural absValue;
		};
	}
}