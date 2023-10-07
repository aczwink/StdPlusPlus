/*
 * Copyright (c) 2021-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Math/DecimalFloat.hpp>
//Local
#include <Std++/Containers/Strings/ConstStringIterator.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;

//Static functions
class DecimalFloatParser
{
public:
	//Members
	String integerPart;
	String fractionalPart;
	String exponentPart;

	//Methods
	void Parse(const String& string)
	{
		auto it = string.begin();
		this->ParseSign(it);
		this->ParseNaturalPart(it, string.end());
		this->ParseFractionalPart(it, string.end());
		this->ParseExponentPart(it, string.end());
	}

private:
	//Methods
	void ParseExponentPart(ConstStringIterator& it, const ConstStringIterator& end)
	{
		if(it == end)
			return;

		if(*it == u8'e' || *it == u8'E')
		{
			++it;

			//parse the exp sign
			if(*it == u8'-')
			{
				this->exponentPart += *it;
				++it;
			}
			else if (*it == u8'+')
				++it;

			while(it != end)
			{
				if(IsValueInInterval(*it, (uint32)u8'0', (uint32)u8'9'))
				{
					this->exponentPart += *it;
					++it;
				}
				else
				{
					//weird symbol
					NOT_IMPLEMENTED_ERROR;
				}
			}
		}
	}

	void ParseFractionalPart(ConstStringIterator& it, const ConstStringIterator& end)
	{
		if(*it == u8'.')
		{
			++it;
			while(it != end)
			{
				if(IsValueInInterval(*it, (uint32)u8'0', (uint32)u8'9'))
				{
					fractionalPart += *it;
					++it;
				}
				else if(*it == u8'e' || *it == u8'E')
				{
					break;
				}
				else
				{
					//weird symbol
					NOT_IMPLEMENTED_ERROR;
				}
			}
		}
	}

	void ParseNaturalPart(ConstStringIterator& it, const ConstStringIterator& end)
	{
		while(it != end)
		{
			if(IsValueInInterval(*it, (uint32)u8'0', (uint32)u8'9'))
			{
				this->integerPart += *it;
				++it;
				continue;
			}

			switch(*it)
			{
				case u8'.':
				case u8'e':
				case u8'E':
					return;
				default:
					//weird symbol
					NOT_IMPLEMENTED_ERROR;
			}
		}
	}

	void ParseSign(ConstStringIterator& it)
	{
		if(*it == u8'-')
		{
			this->integerPart += *it;
			++it;
		}
		else if (*it == u8'+')
			++it;
	}
};

//Constructor
DecimalFloat::DecimalFloat(const String &string)
{
	DecimalFloatParser parser;
	parser.Parse(string);

	this->significand = parser.integerPart + parser.fractionalPart;

	Integer exp = parser.exponentPart.IsEmpty() ? u8"0" : parser.exponentPart;
	this->exponent = exp - parser.fractionalPart.GetLength();
}

//Public methods
String DecimalFloat::ToDecimalFloatingPointString() const
{
	auto sigInt = this->significand.absValue.ToString();
	auto nSigDigits = sigInt.GetLength();
	int64 nIntDigits = (this->exponent + nSigDigits).ClampTo64Bit();

	String natPart, fracPart;
	if(nIntDigits <= 0)
	{
		natPart = u8"0";
		fracPart = u8"." + this->significand.absValue.ToString().SubString(nIntDigits);
	}
	else
	{
		natPart = sigInt.SubString(0, nIntDigits);
		while(natPart.GetLength() < nIntDigits)
			natPart += u8"0";
	}

	String sign = this->significand.isNegative ? u8"-" : u8"";
	return sign + natPart + fracPart;
}

String DecimalFloat::ToScientificNotationString() const
{
	String exp = this->exponent.ToString();
	return this->significand.ToString() + u8"*10^" + exp;
}

String DecimalFloat::ToString() const
{
	String a = this->ToDecimalFloatingPointString();
	String b = this->ToScientificNotationString();

	return a.GetLength() < b.GetLength() ? a : b;
}