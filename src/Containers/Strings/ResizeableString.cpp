/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Strings/ResizeableString.hpp>
//Local
#include <Std++/Char.hpp>
//Namespaces
using namespace StdPlusPlus;

//Local functions
template<typename CharType>
static int8 ParseSign(CharType *&string)
{
	int8 sign = 1;
	if(*string == '-')
	{
		string++;
		sign = -1;
	}
	else if (*string == '+')
		string++;

	return sign;
}

//Public methods
template<typename CharType>
float64 ResizeableString<CharType>::ToFloat() const
{
	if(this->data == nullptr || *this->data == 0)
		return 0; //filter out empty strings

	CharType *current = this->data;

	//skip leading white spaces
	while(IsWhiteSpaceChar(*current))
		current++;

	//parse sign
	int8 sign = ParseSign(current);

	//parse the integer part
	double integerPart = 0;
	while(*current)
	{
		if(IN_RANGE(*current, '0', '9'))
		{
			integerPart = integerPart * 10 + (*current - '0');

			current++;
			continue;
		}
		switch(*current)
		{
			case '.':
			case 'e':
			case 'E':
				goto parseFrac;
				break;
			default:
				//weird symbol
				NOT_IMPLEMENTED_ERROR;
		}
	}

	parseFrac:

	//parse fraction part
	double fractionPart = 0;
	uint32 divisor = 1;

	if(*current == '.')
	{
		current++;
		while(*current)
		{
			if(IN_RANGE(*current, '0', '9'))
			{
				fractionPart = fractionPart*10 + (*current - '0');
				divisor *= 10;
			}
			else if(*current == 'e' || *current == 'E')
			{
				break;
			}
			else
			{
				//weird symbol
				NOT_IMPLEMENTED_ERROR;
			}
			current++;
		}
	}

	//parse exponent
	float64 base = 1;
	if(*current == 'e' || *current == 'E')
	{
		current++;

		int8 expSign = ParseSign(current);
		int16 exp = 0;
		while(*current)
		{
			if(IN_RANGE(*current, '0', '9'))
			{
				exp = exp * 10 + (*current - '0');
			}
			else
			{
				//weird symbol
				NOT_IMPLEMENTED_ERROR;
			}

			current++;
		}

		base = pow(10.0, exp * expSign);
	}

	return sign * (integerPart + fractionPart / divisor) * base;
}

//Explicit instatiation
template class ResizeableString<char>;
template class ResizeableString<uint16>;