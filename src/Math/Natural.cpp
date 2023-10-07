/*
 * Copyright (c) 2017-2018,2021-2022 Amir Czwink (amir130@hotmail.de)
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
 //Class header
#include <Std++/Math/Natural.hpp>
//Local
#include <Std++/Containers/Strings/ConstStringIterator.hpp>
#include <Std++/Containers/Strings/String.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;

//Constructor
Natural::Natural(const String &string)
{
	uint32 pos = 0;
	for(uint32 codePoint : string)
	{
		if(!IsValueInInterval(codePoint, (uint32)u8'0', (uint32)u8'9'))
			NOT_IMPLEMENTED_ERROR;

		uint32 exponent = string.GetLength() - pos - 1;
		*this += Power(Natural(10), Natural(exponent)) * (codePoint - u8'0');
		pos++;
	}
}

//Operators
Math::Natural Math::Natural::operator+(const Natural& other) const
{
	Natural result;
	uint32 index = 0;
	bool carry = false;
	while (true)
	{
		const bool haveLeft = index < this->storage.GetNumberOfSegments();
		const bool haveRight = index < other.storage.GetNumberOfSegments();
		const bool haveMore = haveLeft || haveRight || carry;
		if (!haveMore)
			break;

		uint64 l = haveLeft ? this->storage.GetSegment(index) : 0;
		uint64 r = haveRight ? other.storage.GetSegment(index) : 0;
		
		uint64 current = l + r + (carry ? 1 : 0);
		carry = current < l;
		result.storage.AddSegment(current);
		
		index++;
	}

	return result;
}

Math::Natural Math::Natural::operator-(const Natural& other) const
{
	ASSERT(*this >= other, u8"Let a,b in N: a - b is defined iff a >= b");

	Natural result;
	result.storage.SetNumberOfSegments(this->storage.GetNumberOfSegments());

	bool borrow = false;
	for (uint64 i = 0; i < other.storage.GetNumberOfSegments(); i++)
	{
		uint64 l = this->storage.GetSegment(i);
		uint64 r = other.storage.GetSegment(i);

		uint64 current = l - r - (borrow ? 1 : 0);
		borrow = (current > l) || (borrow && current == l);
		result.storage.SetSegment(i, current);
	}

	for (uint64 i = other.storage.GetNumberOfSegments(); i < this->storage.GetNumberOfSegments(); i++)
	{
		if (borrow)
		{
			result.storage.SetSegment(i, this->storage.GetSegment(i) - 1);
			borrow = this->storage.GetSegment(i) == 0;
		}
		else
			result.storage.SetSegment(i, this->storage.GetSegment(i));
	}

	result.storage.DropLeadingZeros();
	return result;
}

Math::Natural Math::Natural::operator*(const Natural& other) const
{
	Natural result;
	for (uint64 i = 0; i < other.storage.GetNumberOfSegments(); i++)
	{
		for (uint8 j = 0; j < 64; j++)
		{
			if ((other.storage.GetSegment(i) >> j) & 1)
				result += (*this) << (i*64 + j);
		}
	}
	return result;
}

Math::Natural Math::Natural::operator<<(uint64 shift) const
{
	if(shift == 0)
		return *this;

	uint64 blockShift = shift / 64;
	shift %= 64;

	Natural result;
	result.storage.SetNumberOfSegments(this->storage.GetNumberOfSegments() + blockShift);

	uint64 last = 0;
	for (uint64 i = 0; i < this->storage.GetNumberOfSegments(); i++)
	{
		const uint64 leftOver = this->SafeRightShift(last, 64 - shift);
		result.storage.SetSegment(i + blockShift, (this->storage.GetSegment(i) << shift) | leftOver);
		last = this->storage.GetSegment(i);
	}
	const uint64 leftOver = this->SafeRightShift(last, 64 - shift);
	if (leftOver)
	{
		//add what we have shifted away
		result.storage.AddSegment(leftOver);
	}
	for (uint64 i = 0; i < blockShift; i++)
		result.storage.SetSegment(i, 0);

	return result;
}

bool Math::Natural::operator<(const Natural& rhs) const
{
	if (this->storage.GetNumberOfSegments() < rhs.storage.GetNumberOfSegments())
		return true;
	if (this->storage.GetNumberOfSegments() > rhs.storage.GetNumberOfSegments())
		return false;

	for (uint32 i = this->storage.GetNumberOfSegments() - 1; i < this->storage.GetNumberOfSegments(); i--)
	{
		uint64 l = this->storage.GetSegment(i);
		uint64 r = rhs.storage.GetSegment(i);
		if (l < r)
			return true;
		if (l > r)
			return false;
	}
	return false;
}

//Public methods
Tuple<Math::Natural, Math::Natural> Math::Natural::DivMod(const Natural& divisor) const
{
	if (*this < divisor)
		return {Natural(), *this};

	Natural q;
	q.storage.SetNumberOfSegments(this->storage.GetNumberOfSegments());

	Natural r;
	for (uint64 i = this->storage.GetNumberOfSegments() - 1; i < this->storage.GetNumberOfSegments(); i--)
	{
		for (uint8 j = 64 - 1; j < 64; j--)
		{
			r <<= 1;
			uint64 mask = uint64(1) << j;

			if (this->storage.GetSegment(i) & mask)
				++r;

			if (r >= divisor)
			{
				r -= divisor;
				q.storage.SetSegment(i, q.storage.GetSegment(i) | mask);
			}
		}
	}

	q.storage.DropLeadingZeros();

	return { q, r };
}

String Math::Natural::ToString() const
{
	if(this->storage.GetNumberOfSegments() == 0)
		return u8"0";

	const Natural radix = 10;

	String result;
	Natural copy = *this;
	while (copy != 0)
	{
		auto qd = copy.DivMod(radix);
		copy = Move(qd.Get<0>());
		result += (u8'0' + qd.Get<1>().ClampTo64Bit());
	}

	return result.Reversed();
}