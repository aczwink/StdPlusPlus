/*
 * Copyright (c) 2017-2018,2021 Amir Czwink (amir130@hotmail.de)
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
Natural::Natural(const String &string) : lowValue(0)
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
	Natural result = this->lowValue + other.lowValue;
	uint32 index = 0;
	bool carry = false;
	while (true)
	{
		const bool haveLeft = index < this->value.GetNumberOfElements();
		const bool haveRight = index < other.value.GetNumberOfElements();
		const bool haveMore = haveLeft || haveRight || carry;
		if (!haveMore)
			break;

		uint64 l = haveLeft ? this->value[index] : 0;
		uint64 r = haveRight ? other.value[index] : 0;
		
		uint64 current = l + r + (carry ? 1 : 0);
		carry = current < l;
		result.value.Push(current);
		
		index++;
	}

	return result;
}

Math::Natural Math::Natural::operator-(const Natural& other) const
{
	ASSERT(*this >= other, u8"Let a,b in N: a - b is defined iff a >= b");

	uint32 index = other.value.GetNumberOfElements() - 1;
	DynamicArray<uint64> tmp;
	tmp.Resize(this->value.GetNumberOfElements());

	bool borrow = false;
	for (uint64 i = 0; i < other.value.GetNumberOfElements(); i++)
	{
		uint64 l = this->value[i];
		uint64 r = other.value[i];

		uint64 current = l - r - (borrow ? 1 : 0);
		borrow = (current > l) || (borrow && current == l);
		tmp[i] = current;
	}

	for (uint64 i = other.value.GetNumberOfElements(); i < this->value.GetNumberOfElements(); i++)
	{
		if (borrow)
		{
			tmp[i] = this->value[i] - 1;
			borrow = this->value[i] == 0;
		}
		else
			tmp[i] = this->value[i];
	}
	
	//drop leading zeros
	while (!tmp.IsEmpty() && (tmp.Last() == 0))
		tmp.Pop();

	Natural result;
	result.value = Move(tmp);
	return result;
}

Math::Natural Math::Natural::operator*(const Natural& other) const
{
	Natural result = this->lowValue * other.lowValue;
	for (uint64 i = 0; i < other.value.GetNumberOfElements(); i++)
	{
		for (uint8 j = 0; j < 64; j++)
		{
			if ((other.value[i] >> j) & 1)
				result += (*this) << (i*64 + j);
		}
	}
	return result;
}

Math::Natural Math::Natural::operator<<(uint64 shift) const
{
	if(shift == 0)
		return *this;
	if (this->value.IsEmpty())
		return Natural(); //zero stays zero

	uint64 blockShift = shift / 64;
	shift %= 64;

	Natural result;
	result.value.Resize(this->value.GetNumberOfElements() + blockShift);
	uint64 last = 0;
	const uint64 mask = ~((uint64(1) << (64 - shift)) - 1);
	for (uint64 i = 0; i < this->value.GetNumberOfElements(); i++)
	{
		const uint64 leftOver = last >> (64 - shift);
		result.value[i + blockShift] = (this->value[i] << shift) | leftOver;
		last = this->value[i];
	}
	const uint64 leftOver = last >> (64 - shift);
	if (leftOver)
	{
		//add what we have shifted away
		result.value.Push(leftOver);
	}
	for (uint64 i = 0; i < blockShift; i++)
		result.value[i] = 0;

	return result;
}

bool Math::Natural::operator<(const Natural& other) const
{
	if (this->value.GetNumberOfElements() < other.value.GetNumberOfElements())
		return true;
	if (this->value.GetNumberOfElements() > other.value.GetNumberOfElements())
		return false;

	for (uint32 i = this->value.GetNumberOfElements() - 1; i < this->value.GetNumberOfElements(); i--)
	{
		if (this->value[i] < other.value[i])
			return true;
		if (this->value[i] > other.value[i])
			return false;
	}
	return this->lowValue < other.lowValue;
}

//Public methods
Tuple<Math::Natural, Math::Natural> Math::Natural::DivMod(const Natural& divisor) const
{
	if (*this < divisor)
		return {Natural(), *this};

	Natural q;
	Natural r;
	for (uint64 i = this->value.GetNumberOfElements() - 1; i < this->value.GetNumberOfElements(); i--)
	{
		for (uint8 j = 64 - 1; j < 64; j--)
		{
			r <<= 1;
			uint64 mask = uint64(1) << j;

			if (this->value[i] & mask)
				++r;

			if (r >= divisor)
			{
				r -= divisor;
				if (i >= q.value.GetNumberOfElements())
				{
					uint32 oldSize = q.value.GetNumberOfElements();
					q.value.Resize(i + 1);
					for (uint32 i = oldSize; i < q.value.GetNumberOfElements(); i++)
						q.value[i] = 0;
				}
				q.value[i] |= mask;
			}
		}
	}

	while (!q.value.Last())
		q.value.Pop();

	return { q, r };
}

String Math::Natural::ToString() const
{
	if (this->value.IsEmpty())
		return String::Number(this->lowValue);

	const Natural radix = 10;

	String result;
	Natural copy = *this;
	while (copy != 0)
	{
		auto qd = copy.DivMod(radix);
		copy = Move(qd.Get<0>());
		if (qd.Get<1>().value.IsEmpty())
			result += u8'0';
		else
			result += (u8'0' + qd.Get<1>().value[0]);
	}

	return result.Reversed();
}